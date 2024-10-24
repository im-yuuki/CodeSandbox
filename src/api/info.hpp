#ifndef INFO_HPP
#define INFO_HPP

#include <crow/http_response.h>
#include <nlohmann/json.hpp>

#include "../data/problems.hpp"
#include "../data/storage.hpp"
#include "../modules/register.hpp"

namespace api {

	inline crow::response get_all_modules() {
		std::vector<std::string> handlers;
		for (const auto& key : modules::modules | std::views::keys) {
			handlers.push_back(key);
		}
		const nlohmann::json j = handlers;
		crow::response resp = {200, j.dump()};
		resp.add_header("Content-Type", "application/json");
		return resp;
	}

	inline crow::response get_all_problems() {
		std::vector<std::string> problem_ids;
		for (const auto& problem : data::problem_list) {
			problem_ids.push_back(problem.id);
		}
		const nlohmann::json j = problem_ids;
		crow::response resp = {200, j.dump()};
		resp.add_header("Content-Type", "application/json");
		return resp;
	}

}

#endif
