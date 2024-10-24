#ifndef SUBMIT_HPP
#define SUBMIT_HPP

#include <crow/http_response.h>
#include <crow/multipart.h>

#include "../data/storage.hpp"

namespace api {

	static std::mutex submit_mutex;

	inline crow::response submit(const crow::request& req) {
		std::lock_guard lock(submit_mutex); // Lock the mutex

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
		if (id.empty() || problem_id.empty() || target_module.empty()) {
			return {400, "Missing required fields"};
		}

		data::Submission submission(id, problem_id, file_content, target_module);

		try {
			const data::Problem* problem = data::get_problem(submission.problem_id);
			modules::IModules* handler = modules::create_handler(&submission, problem);
			handler->run();
			submission = handler->submission;
			handler->cleanup();
		} catch (const std::exception& e) {
			submission.status = data::submission_status::InternalError;
			submission.message = e.what();
		}

		nlohmann::json j = {
			{"id", submission.id},
			{"status", repr(submission.status)},
			{"message", submission.message}
		};

		crow::response resp = {200, j.dump()};
		resp.add_header("Content-Type", "application/json");
		return resp;
	}

}

#endif

