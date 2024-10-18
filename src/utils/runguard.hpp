#ifndef RUNGUARD_HPP
#define RUNGUARD_HPP

#include <cstdlib>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sstream>
#include "logging.hpp"

namespace utils {
	const static auto logger = logging::create_logger("runguard");

	class RunGuard {
		unsigned time_limit_secs;
		unsigned memory_limit_mb;
		int status = EXIT_SUCCESS;

	public:
		RunGuard(const unsigned time_limit_secs, const unsigned memory_limit_mb)
			: time_limit_secs(time_limit_secs), memory_limit_mb(memory_limit_mb) {
		}

		void run(const char* command, std::string& input, std::stringstream& output) {
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

				// Reset signal handler for SIGXCPU
				struct sigaction sa{};
				sa.sa_handler = SIG_DFL;
				sigaction(SIGXCPU, &sa, nullptr);

				char* args[] = {const_cast<char*>(command), nullptr};

				// Set resource limits
				if (memory_limit_mb) {
					rlimit mem_limit{};
					mem_limit.rlim_cur = memory_limit_mb * 1024 * 1024;
					mem_limit.rlim_max = memory_limit_mb * 1024 * 1024;
					if (setrlimit(RLIMIT_AS, &mem_limit) != 0) exit(EXIT_FAILURE);
				}
				if (time_limit_secs) {
					rlimit cpu_limit{};
					cpu_limit.rlim_cur = time_limit_secs;
					cpu_limit.rlim_max = time_limit_secs;
					if (setrlimit(RLIMIT_CPU, &cpu_limit) != 0) exit(EXIT_FAILURE);
				}

				execvp(command, args);

				exit(EXIT_FAILURE);
			}

			if (child_pid > 0) {
				// In parent process
				close(pipe_in[0]); // Close read end of input pipe
				close(pipe_out[1]); // Close write end of output pipe

				// Write to the child process's stdin
				write(pipe_in[1], input.c_str(), input.size());
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

				if (WIFEXITED(status)) logger->info("Process exited with code {}", WEXITSTATUS(status));
				else if (WIFSIGNALED(status)) logger->error("Process terminated by signal {}", WTERMSIG(status));
			}
			else logger->error("Failed to fork process");
		}
	};
}

#endif
