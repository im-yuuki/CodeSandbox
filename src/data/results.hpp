#ifndef RESULTS_HPP
#define RESULTS_HPP
#include <string>

#include "enums.hpp"

namespace data {

	struct Result {
		std::string submission_id;
		submission_status::Enum status;
		std::string message;
	};

}

#endif
