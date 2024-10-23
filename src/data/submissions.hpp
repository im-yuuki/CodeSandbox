#ifndef SUBMISSION_H
#define SUBMISSION_H

#include <string>
#include "enums.hpp"

using namespace std;

namespace data {

	struct Submission {
		string submission_id;
		string problem_id;
		byte* content = nullptr;
		string profile;
		data::submission_status::Enum status = data::submission_status::Pending;
		string message;

		Submission(
			const string& submission_id,
			const string& problem_id,
			const string& profile
			) {
			this->submission_id = submission_id;
			this->problem_id = problem_id;
			this->profile = profile;
		}
	};

}

#endif
