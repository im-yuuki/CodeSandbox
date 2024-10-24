#ifndef RESULT_HPP
#define RESULT_HPP

#include <crow/http_response.h>
#include <crow/http_request.h>
#include <crow/query_string.h>

#include "../data/results.hpp"
#include "../data/storage.hpp"

namespace api {

	inline crow::response result(const crow::request& req) {
		const auto qs = crow::query_string(req.body);
		const string submission_id = qs.get("submission_id");
		if (submission_id.empty()) return {400, "Submission ID is required"};
		try {
			const auto [id, status, message] = data::get_result(submission_id);
			const nlohmann::json result = {
				{"id", id},
				{"status", status},
				{"message", message}
			};
			return {200, result.dump()};
		} catch (const std::exception& e) {
			return {404, e.what()};
		}
	}

}

#endif
