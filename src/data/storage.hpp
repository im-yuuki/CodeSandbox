#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <vector>

#include "problems.hpp"
#include "results.hpp"
#include "../utils/logging.hpp"
#include "../utils/env.hpp"

namespace data {

	static std::vector<Result> result_list;
	static std::vector<Problem> problem_list;

	inline Result get_result(const std::string& id) {
		for (const auto& result : result_list) {
			if (result.id == id) return result;
		}
		throw std::runtime_error("Result ID is not exist in this node");
	}

	inline Problem get_problem(const std::string& id) {
		for (const auto& problem : problem_list) {
			if (problem.id == id) return problem;
		}
		throw std::runtime_error("Problem ID is not available in this node");
	}

	inline Problem load_problem(const std::string& id) {
		std::ifstream file("problems/" + id + "/config.cfg");
		int time_limit_secs = 0, memory_limit_mb = 0, test_cases_count = 0;
		std::string line;
		while (getline(file, line)) {
			if (line.empty() || line[0] == '#') continue;
			if (const size_t delimiter_pos = line.find('='); delimiter_pos != std::string::npos) {
				// Parse configuration file
				std::string key = utils::trim(line.substr(0, delimiter_pos));
				std::string value = utils::trim(line.substr(delimiter_pos + 1));
				if (key == "TIME_LIMIT") {
					time_limit_secs = stoi(value);
				} else if (key == "MEMORY_LIMIT") {
					memory_limit_mb = stoi(value);
				} else if (key == "TEST_CASES") {
					test_cases_count = stoi(value);
				}
			}
		}
		file.close();
		if (test_cases_count == 0) throw std::runtime_error("Invalid problem configuration");
		Problem problem(id, time_limit_secs, memory_limit_mb);
		// Load test cases
		for (int i = 1; i <= test_cases_count; i++) problem.test_cases.emplace_back(id, i);
		return problem;
	}

	inline void scan_problems() {
		const auto logger = logging::create_logger("problems");
		// Create the problems directory if it does not exist
		if (!std::filesystem::exists("problems")) {
			std::filesystem::create_directory("problems");
		}
		unsigned problems_count = 0;
		for (const auto& entry : std::filesystem::directory_iterator("problems")) {
			try {
				if (!entry.is_directory()) continue;
				const std::string id = entry.path().filename().string();
				problem_list.push_back(load_problem(id));
				problems_count++;
			} catch (const std::exception& e) {
				logger->error("Failed to load problem {}: {}", entry.path().filename().string(), e.what());
			}
		}
		logger->info("Indexed {} problems", problem_list.size());
	}

}

#endif
