#ifndef SUBMISSION_H
#define SUBMISSION_H

#include <string>
#include "enums.hpp"

namespace data {

	struct Submission {
		std::string id;
		std::string problem_id;
		std::string file_content;
		std::string target_module;
		submission_status::Enum status = submission_status::Queued;
		std::string message;

		Submission() = default;

		Submission(
			const std::string& submission_id,
			const std::string& problem_id,
			const std::string& file_content,
			const std::string& target_module
			) {
			this->id = submission_id;
			this->problem_id = problem_id;
			this->file_content = file_content;
			this->target_module = target_module;
		}
	};

}

#endif
