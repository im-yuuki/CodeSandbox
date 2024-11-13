#ifndef RUNGUARD_HPP
#define RUNGUARD_HPP

#include <cstdlib>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sstream>
#include "logging.hpp"

namespace utils {
	const static auto logger = logging::create_logger("runguard");

	class RunGuard {
		unsigned time_limit_secs;
		unsigned memory_limit_mb;

	public:
		int status = EXIT_FAILURE;
		std::string message;

		RunGuard(const unsigned time_limit_secs, const unsigned memory_limit_mb)
			: time_limit_secs(time_limit_secs), memory_limit_mb(memory_limit_mb) {
		}

		void run(const char* command, const std::string& input, std::stringstream& output) {
			int pipe_in[2];
			int pipe_out[2];

			if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1) {
				logger->error("Failed to create pipes");
				return;
			}

			const pid_t child_pid = fork();

			if (child_pid == 0) {
				// In child process
				close(pipe_in[1]); // Close write end of input pipe
				close(pipe_out[0]); // Close read end of output pipe

				// Redirect stdin and stdout
				dup2(pipe_in[0], STDIN_FILENO);
				dup2(pipe_out[1], STDOUT_FILENO);

				int arg_idx = 0;
				int space_idx = 0;
				int i = 0;
				// char[32] probably enough to not get overflowed
				char* args[3] = {new char[32], nullptr, nullptr};
				for (i = 0; command[i] != '\0'; i++) {
					args[arg_idx][i - space_idx] = command[i];
					if (command[i] == ' ') {
						args[arg_idx][i] = '\0';
						arg_idx++;
						args[arg_idx] = new char[32];
						space_idx = i;
					}
				}
				args[arg_idx][i - space_idx] = '\0';
				if (!space_idx) {
					args[0] = const_cast<char*>(command);
				}

				// Set resource limits
				if (memory_limit_mb) {
					rlimit mem_limit{};
					mem_limit.rlim_cur = memory_limit_mb * 1048576;
					mem_limit.rlim_max = memory_limit_mb * 1048576;
					if (setrlimit(RLIMIT_AS, &mem_limit) != 0) exit(EXIT_FAILURE);
				}
				if (time_limit_secs) {
					rlimit cpu_limit{};
					cpu_limit.rlim_cur = time_limit_secs;
					cpu_limit.rlim_max = time_limit_secs;
					if (setrlimit(RLIMIT_CPU, &cpu_limit) != 0) exit(EXIT_FAILURE);
					alarm(time_limit_secs);
				}

				execvp(args[0], args);

				exit(EXIT_FAILURE);
			}

			if (child_pid > 0) {
				// In parent process
				close(pipe_in[0]); // Close read end of input pipe
				close(pipe_out[1]); // Close write end of output pipe

				// Write to the child process's stdin
				if (write(pipe_in[1], input.c_str(), input.size()) == -1) logger->error("Failed to write input data to subprocess input");
				close(pipe_in[1]); // Close write end after writing

				// Read from the child process's stdout
				char buffer[256];
				ssize_t count;
				while ((count = read(pipe_out[0], buffer, sizeof(buffer) - 1)) > 0) {
					buffer[count] = '\0';
					output << buffer; // Append the data from child process to output stream
				}
				close(pipe_out[0]); // Close read end after reading

				waitpid(child_pid, &status, 0);

				if (WIFEXITED(status)) message = "Process exited with code " + std::to_string(WEXITSTATUS(status));
				else if (WIFSIGNALED(status)) message = "Process terminated by signal " + std::to_string(WTERMSIG(status));
			}
			else message = "Failed to fork process";
			if (status != EXIT_SUCCESS) logger->error(message);
		}
	};
}

#endif
