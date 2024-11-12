#ifndef RUST_PROFILE_HPP
#define RUST_PROFILE_HPP
#include "abstract.hpp"

namespace modules {

    class Rust final : public IModules {
    private:
        const std::string compile_command = utils::get_env("RUST_COMPILE_COMMAND", "rustc");
        const std::string default_args = "-C opt-level=3";

        void compile() override {
            if (submission.status != data::submission_status::Running) return;
            std::ofstream source_file(work_dir + "/main.rs", std::ios::binary);
            source_file << submission.file_content;
            source_file.close();
            std::string standard;
            const std::string command = compile_command
                + ' ' + default_args
                + ' ' + work_dir + "/main.rs"
                + " -o " + work_dir + "/main.out";
            if (system(command.c_str()) != 0) {
                submission.status = data::submission_status::CompilationError;
                submission.message = "Compilation command exit with non-zero status";
            }
        }

        void test(const std::string& input, const std::string& output) override {
            utils::RunGuard run_guard(problem.time_limit_secs, problem.memory_limit_mb);
            std::stringstream output_stream;
            run_guard.run((work_dir + "/main.out").c_str(), input, output_stream);
            if (WIFEXITED(run_guard.status)) {
                if (WEXITSTATUS(run_guard.status) == EXIT_SUCCESS) {
                    if (!utils::token_compare(output_stream, output))
                        submission.status = data::submission_status::WrongAnswer;
                    else return;
                }
            }
            else if (WIFSIGNALED(run_guard.status)) {
                if (WTERMSIG(run_guard.status) == SIGXCPU || WTERMSIG(run_guard.status) == SIGALRM)
                    submission.status = data::submission_status::TimeLimitExceeded;
                else if (WTERMSIG(run_guard.status) == SIGKILL)
                    submission.status = data::submission_status::MemoryLimitExceeded;
            }
            else {
                submission.status = data::submission_status::RuntimeError;
                submission.message = run_guard.message;
            }
        }

    public:
        Rust(const data::Submission* submission, const data::Problem* problem) : IModules(submission, problem) {}
    };

}

#endif
