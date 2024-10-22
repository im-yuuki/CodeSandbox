#ifndef INFO_HPP
#define INFO_HPP

#include <crow/http_response.h>
#include <nlohmann/json.hpp>

#include "../data/problems.hpp"
#include "../handlers/register.hpp"

namespace api {

	inline crow::response get_all_handlers() {
		vector<string> handlers;
		for (const auto& key : handlers::handlers | views::keys) {
			handlers.push_back(key);
		}
		const nlohmann::json j = handlers;
		return {200, j.dump()};
	}

	inline crow::response get_all_problems() {
		vector<string> problem_ids;
		for (const auto& problem : data::problem_list) {
			problem_ids.push_back(problem.id);
		}
		const nlohmann::json j = problem_ids;
		return {200, j.dump()};
	}

}

#endif
