#ifndef RUNGUARD_HPP
#define RUNGUARD_HPP

#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>
#include <cstdio>
#include <fcntl.h>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#elif __linux__
#include <unistd.h>
#include <fstream>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

class RunGuard {
public:
    RunGuard(const long memory_limit_mb, const int time_limit_sec)
        : memory_limit_kib(memory_limit_mb * 1024), time_limit_sec(time_limit_sec), child_pid(-1) {}

    bool start(const char* command, const int input_fd = STDIN_FILENO, const int output_fd = STDOUT_FILENO, const int error_fd = STDERR_FILENO) {
#ifdef _WIN32
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        if (!CreateProcess(nullptr, const_cast<char*>(command), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &pi)) {
            std::cerr << "CreateProcess failed!" << std::endl;
            return false;
        }

        child_pid = pi.dwProcessId;
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
#else
        if ((child_pid = fork()) == -1) {
            std::cerr << "Fork failed!" << std::endl;
            return false;
        }

        if (child_pid == 0) {  // Child process
            // Redirect streams
            if (input_fd != STDIN_FILENO) {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            if (output_fd != STDOUT_FILENO) {
                dup2(output_fd, STDOUT_FILENO);
                close(output_fd);
            }
            if (error_fd != STDERR_FILENO) {
                dup2(error_fd, STDERR_FILENO);
                close(error_fd);
            }

            execlp(command, command, nullptr);
            perror("exec failed");
            exit(1);
        }
#endif
        monitor_thread = std::thread(&RunGuard::monitor_resource_limits, this);
        monitor_thread.detach();
        return true;
    }

    void wait_for_completion() const {
        if (child_pid != -1) {
#ifdef _WIN32
            HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, child_pid);
            if (process) {
                WaitForSingleObject(process, INFINITE);
                CloseHandle(process);
            }
            DWORD exit_code;
            process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, child_pid);
            if (process) {
                GetExitCodeProcess(process, &exit_code);
                CloseHandle(process);
                if (exit_code == STILL_ACTIVE) {
                    std::cerr << "Child process was killed due to resource limit violation." << std::endl;
                } else {
                    std::cout << "Child process finished normally." << std::endl;
                }
            }
#else
            waitpid(child_pid, &status, 0);
            if (WIFSIGNALED(status) && WTERMSIG(status) == SIGKILL) {
                std::cerr << "Child process was killed due to resource limit violation." << std::endl;
            } else {
                std::cout << "Child process finished normally." << std::endl;
            }
#endif
        }
    }

private:
    long memory_limit_kib;
    int time_limit_sec;
    pid_t child_pid;
    std::thread monitor_thread;

    static long get_memory_usage(pid_t pid) {
#ifdef _WIN32
        PROCESS_MEMORY_COUNTERS pmc;
        HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

        if (process == nullptr) {
            return -1;
        }

        if (GetProcessMemoryInfo(process, &pmc, sizeof(pmc))) {
            CloseHandle(process);
            return pmc.WorkingSetSize / 1024;
        }

        CloseHandle(process);
        return -1;

#elif __linux__
        std::ifstream status_file("/proc/" + std::to_string(pid) + "/status");
        std::string line;
        long mem_usage_kib = 0;

        while (std::getline(status_file, line)) {
            if (line.find("VmRSS:") != std::string::npos) {  // Look for resident memory
                std::istringstream iss(line);
                std::string label;
                iss >> label >> mem_usage_kib;  // Read memory in KiB
                break;
            }
        }

        return mem_usage_kib;  // Return memory in KiB
#else
        return -1;
#endif
    }

    void monitor_resource_limits() const {
        const auto start_time = std::chrono::steady_clock::now();

        while (true) {
            // Check memory usage
            if (const long mem_usage_kib = get_memory_usage(child_pid); mem_usage_kib > 0 && mem_usage_kib > memory_limit_kib) {
                std::cerr << "Memory limit exceeded! Killing process." << std::endl;
#ifdef _WIN32
                HANDLE process = OpenProcess(PROCESS_TERMINATE, FALSE, child_pid);
                if (process) {
                    TerminateProcess(process, 1);
                    CloseHandle(process);
                }
#else
                kill(child_pid, SIGKILL);
#endif
                break;
            }

            auto elapsed_time = std::chrono::steady_clock::now() - start_time;
            if (std::chrono::duration_cast<std::chrono::seconds>(elapsed_time).count() >= time_limit_sec) {
                std::cerr << "Time limit exceeded! Killing process." << std::endl;
#ifdef _WIN32
                HANDLE process = OpenProcess(PROCESS_TERMINATE, FALSE, child_pid);
                if (process) {
                    TerminateProcess(process, 1);
                    CloseHandle(process);
                }
#else
                kill(child_pid, SIGKILL);
#endif
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
};

#endif