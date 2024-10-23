#ifndef PROFILE_ABS_HPP
#define PROFILE_ABS_HPP

#include <string>
#include <utility>
#include <filesystem>

#include "../data/problems.hpp"
#include "../data/submissions.hpp"
#include "../utils/logging.hpp"
#include "../utils/random.hpp"

namespace handlers {

	static auto logger = logging::create_logger("handlers");

	class IHandler {
	private:
		virtual void compile() = 0;
		virtual void test(string input, string output) = 0;

	public:
		data::Submission submission;
		data::Problem problem;
		std::string work_dir;
		std::string variant = "default";

		virtual vector<string> get_variants() = 0;

		IHandler(data::Submission submission, data::Problem problem) : submission(std::move(submission)), problem(std::move(problem)) {
			// Create a temporary directory for the submission
			this->work_dir = "run/" + utils::random_dir_name();
			try {
				std::filesystem::create_directory(this->work_dir);
			} catch (const std::exception& e) {
				logger->info("Error while creating run space for submission " + submission.submission_id + ": " + e.what());
				submission.status = data::submission_status::InternalError;
				submission.message = e.what();
			}
		};

		void run() {
			if (submission.status != data::submission_status::Queued) return;
			submission.status = data::submission_status::Running;
			compile();
			int tc_count = 0;
			// Run test cases
			if (submission.status != data::submission_status::Running) goto end_run;
			for (const auto& test_case : problem.test_cases) {
				test(test_case.input, test_case.output);
				if (submission.status == data::submission_status::WrongAnswer) submission.message = "Wrong answer on test case " + std::to_string(++tc_count);
				if (submission.status != data::submission_status::Running) goto end_run;
			}
			submission.status = data::submission_status::Accepted;
			submission.message = "All test cases passed";
			end_run:
			logger->info("Submission " + submission.submission_id + " finished: " + std::to_string(submission.status));
		}

		virtual ~IHandler() {
			try {
				std::filesystem::remove_all(this->work_dir);
			} catch (const std::exception& e) {
				logger->info("Error while cleaning run space for submission " + submission.submission_id + ": " + e.what());
			}
		}
	};

}

#endif
