#ifndef PROBLEMS_HPP
#define PROBLEMS_HPP

#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <filesystem>
#include "../utils/env.hpp"
#include "../utils/logging.hpp"
using namespace std;

namespace data {

	struct Problem {
		string id;
		int time_limit_ms, memory_limit_mb, test_cases_count;

		Problem(string id, const int time_limit_ms, const int memory_limit_mb, const int test_cases_count) {
			this->id = std::move(id);
			this->time_limit_ms = time_limit_ms;
			this->memory_limit_mb = memory_limit_mb;
			this->test_cases_count = test_cases_count;
		}
	};

	static vector<Problem> problem_list;

	inline Problem load_problem(const string& id) {
		std::ifstream file("problems/" + id + "/config.txt");
		int time_limit_ms = 0, memory_limit_mb = 0, test_cases_count = 0;
		string line;
		while (getline(file, line)) {
			if (line.empty() || line[0] == '#') continue;
			if (const size_t delimiter_pos = line.find('='); delimiter_pos != string::npos) {
				string key = utils::trim(line.substr(0, delimiter_pos));
				string value = utils::trim(line.substr(delimiter_pos + 1));
				if (key == "TIME_LIMIT") {
					time_limit_ms = stoi(value);
				} else if (key == "MEMORY_LIMIT") {
					memory_limit_mb = stoi(value);
				} else if (key == "TEST_CASES") {
					test_cases_count = stoi(value);
				}
			}
		}
		file.close();
		if (test_cases_count == 0) throw std::runtime_error("Invalid problem configuration");
		// Verify that the test cases exist
		for (int i = 1; i <= test_cases_count; i++) {
			if (!std::filesystem::exists("problems/" + id + "/testcases" + to_string(i) + ".in") ||
				!std::filesystem::exists("problems/" + id + "/testcases" + to_string(i) + ".out"))
				throw std::runtime_error("Missing test case " + to_string(i));
		}
		return {id, time_limit_ms, memory_limit_mb, test_cases_count};
	}

	inline void scan_problems() {
		const auto logger = logging::create_logger("problems");
		unsigned problems_count = 0;
		for (const auto& entry : std::filesystem::directory_iterator("problems")) {
			try {
				if (!entry.is_directory()) continue;
				const string id = entry.path().filename().string();
				problem_list.push_back(load_problem(id));
				logger->info("Loaded problem with ID: {}", id);
			} catch (const std::exception& e) {
				logger->error("Failed to load problem {}: {}", entry.path().filename().string(), e.what());
			}
		}
		logger->info("Loaded {} problems", problem_list.size());
	}

}

#endif
