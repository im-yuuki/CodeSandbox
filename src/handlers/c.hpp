#ifndef C_PROFILE_HPP
#define C_PROFILE_HPP

#include "abstract.hpp"
#include "../utils/compare.hpp"
#include "../utils/env.hpp"
#include "../utils/runguard.hpp"

namespace handlers {

	class C final : public IHandler {
	private:
		const string compile_command = utils::get_env("C_COMPILE_COMMAND", "gcc");
		const string default_args = "-O3 -static -DONLINE_JUDGE -lm -o main.out";

		void compile() override {
			if (submission.status != data::submission_status::Running) return;
			if (submission.content == nullptr) {
				submission.status = data::submission_status::InternalError;
				submission.message = "Submission content is null";
				return;
			}
			std::ofstream source_file(work_dir + "/main.c");
			source_file << submission.content;
			source_file.close();
			string standard;
			if (variant == "c98") standard = "-std=c98";
			else if (variant == "c11") standard = "-std=c11";
			else standard = "-std=c17";
			const string command = compile_command + ' ' + standard + ' ' + default_args + ' ' + work_dir + "/main.c";
			if (system(command.c_str()) != 0) {
				submission.status = data::submission_status::CompilationError;
				submission.message = "Compilation command exit with non-zero status";
			}
		}

		void test(const string& input, const string& output) override {
			utils::RunGuard run_guard(problem.time_limit_secs, problem.memory_limit_mb);
			std::stringstream output_stream;
			run_guard.run((work_dir + "/main.out").c_str(), input, output_stream);
			if (run_guard.status == EXIT_SUCCESS) {
				if (!utils::token_compare(output_stream, output)) {
					submission.status = data::submission_status::WrongAnswer;
				}
			} else if (run_guard.status == SIGXCPU) {
				submission.status = data::submission_status::TimeLimitExceeded;
			} else if (run_guard.status == SIGXFSZ) {
				submission.status = data::submission_status::MemoryLimitExceeded;
			} else {
				submission.status = data::submission_status::RuntimeError;
			}
		}

	public:
		C(data::Submission submission, data::Problem problem) : IHandler(std::move(submission), std::move(problem)) {}
	};

}

#endif