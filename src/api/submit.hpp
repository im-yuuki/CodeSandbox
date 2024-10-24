#ifndef SUBMIT_HPP
#define SUBMIT_HPP

#include <crow/http_response.h>
#include <crow/multipart.h>

#include "../utils/queue.hpp"

namespace api {

	inline crow::response submit(const crow::request& req) {
		std::string id;
		std::string problem_id;
		uint8_t* file_content = nullptr;
		std::string target_module;

		for (const auto msg = crow::multipart::message(req); const auto& part : msg.parts) {
			if (auto part_name = part.get_header_object("Content-Disposition").value; part_name == "id") {
				id = part.body;
			} else if (part_name == "problem_id") {
				problem_id = part.body;
			} else if (part_name == "file") {
				const auto content = new std::vector<uint8_t>(part.body.begin(), part.body.end());
				file_content = content->data();
				delete content;
			} else if (part_name == "target_module") {
				target_module = part.body;
			}
		}
		if (id.empty() || problem_id.empty() || file_content == nullptr || target_module.empty()) {
			return {400, "Missing required fields"};
			delete file_content;
		}
		::queue::submission_queue.enqueue(data::Submission(id, problem_id, file_content, target_module));
		return {200, "Submission received"};

	}

}

#endif

