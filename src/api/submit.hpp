#ifndef SUBMIT_HPP
#define SUBMIT_HPP

#include <crow/http_response.h>
#include <crow/multipart.h>

#include "../utils/queue.hpp"

namespace api {

	inline crow::response submit(const crow::request& req) {
		std::string id;
		std::string problem_id;
		std::string file_content;
		std::string target_module;

		for (const auto msg = crow::multipart::message(req); const auto&[part_name, snd] : msg.part_map) {
			const auto&[headers, body] = snd;

			if (part_name == "id")  id = body;
			else if (part_name == "problem_id") problem_id = body;
			else if (part_name == "file") file_content = body;
			else if (part_name == "target_module") target_module = body;
		}
		if (id.empty() || problem_id.empty() || file_content.empty() || target_module.empty()) {
			return {400, "Missing required fields"};
		}
		::queue::submission_queue.enqueue(data::Submission(id, problem_id, file_content, target_module));
		return {200, "Submission received"};

	}

}

#endif

