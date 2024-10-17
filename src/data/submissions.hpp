#ifndef SUBMISSION_H
#define SUBMISSION_H

#include <string>
#include "enums.hpp"

using namespace std;

struct Submission {
	string submission_id;
	string problem_id;
	byte* file = nullptr;
	data::profile::Enum profile;
	data::submission_status::Enum status = data::submission_status::pending;
	string message;

	Submission(
		const string& submission_id,
		const string& problem_id,
		const data::profile::Enum profile
		) {
		this->submission_id = submission_id;
		this->problem_id = problem_id;
		this->profile = profile;
	}
};


#endif
