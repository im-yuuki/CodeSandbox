#ifndef C_PROFILE_HPP
#define C_PROFILE_HPP

#include "abstract.hpp"
#include "../utils/compare.hpp"
#include "../utils/env.hpp"
#include "../utils/runguard.hpp"

namespace handlers {

	class C final : public IModules {
	private:
		const std::string compile_command = utils::get_env("C_COMPILE_COMMAND", "gcc");
		const std::string default_args = "-O3 -static -DONLINE_JUDGE -lm -o main.out";

		void compile() override {
			if (submission.status != data::submission_status::Running) return;
			std::ofstream source_file(work_dir + "/main.c");
			source_file << submission.file_content;
			source_file.close();
			std::string standard;
			if (variant == "c98") standard = "-std=c98";
			else if (variant == "c11") standard = "-std=c11";
			else standard = "-std=c17";
			const std::string command = compile_command + ' ' + standard + ' ' + default_args + ' ' + work_dir + "/main.c";
			if (system(command.c_str()) != 0) {
				submission.status = data::submission_status::CompilationError;
				submission.message = "Compilation command exit with non-zero status";
			}
		}

		void test(const std::string& input, const std::string& output) override {
			utils::RunGuard run_guard(problem.time_limit_secs, problem.memory_limit_mb);
			std::stringstream output_stream;
			run_guard.run((work_dir + "/main.out").c_str(), input, output_stream);
			if (run_guard.status == EXIT_SUCCESS) {
				if (!utils::token_compare(output_stream, output)) {
					submission.status = data::submission_status::WrongAnswer;
				}
			} else if (run_guard.status == SIGXCPU) {
				submission.status = data::submission_status::TimeLimitExceeded;
				submission.message = "Time limit exceeded";
			} else if (run_guard.status == SIGXFSZ) {
				submission.status = data::submission_status::MemoryLimitExceeded;
				submission.message = "Memory limit exceeded";
			} else {
				submission.status = data::submission_status::RuntimeError;
				submission.message = run_guard.message;
			}
		}

	public:
		C(data::Submission submission, data::Problem problem) : IModules(std::move(submission), std::move(problem)) {}
	};

}

#endif