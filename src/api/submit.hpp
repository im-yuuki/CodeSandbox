#ifndef SUBMIT_HPP
#define SUBMIT_HPP

#include <crow/http_response.h>
#include <crow/multipart.h>

#include "../data/storage.hpp"

namespace api {

	static std::mutex submit_mutex;
	static auto logger = logging::create_logger("api");

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
		if (id.empty() || problem_id.empty() || target_module.empty()) {
			return {400, "Missing required fields"};
		}

		data::Submission submission(id, problem_id, file_content, target_module);
		logger->info(
			"New submission ID {} from {}, using module {}",
			submission.id, req.remote_ip_address, target_module
			);

		{
			std::lock_guard lock(submit_mutex);
			modules::IModules* handler = nullptr;

			try {
				const data::Problem* problem = data::get_problem(submission.problem_id);
				handler = modules::create_handler(&submission, problem);
				handler->run();
				submission = handler->submission;
			} catch (const std::exception& e) {
				submission.status = data::submission_status::InternalError;
				submission.message = e.what();
			}

			if (handler != nullptr) handler->cleanup();
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

