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

	struct TestCase {
		string input, output;
		TestCase(const std::string& problem_id, int test_case_no) {
		    std::ifstream input_file("problems/" + problem_id + "/testcases/" + std::to_string(test_case_no) + ".in");
		    if (!input_file.is_open()) throw std::runtime_error("Failed to open input file of test case " + std::to_string(test_case_no));

		    std::ifstream output_file("problems/" + problem_id + "/testcases/" + std::to_string(test_case_no) + ".out");;
		    if (!output_file.is_open()) throw std::runtime_error("Failed to open output file of test case " + std::to_string(test_case_no));

		    this->input = std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
		    this->output = std::string((std::istreambuf_iterator<char>(output_file)), std::istreambuf_iterator<char>());

			input_file.close();
			output_file.close();
		}
	};

	struct Problem {
		string id;
		int time_limit_ms, memory_limit_mb;
		vector<TestCase> test_cases;

		Problem(string id, const int time_limit_ms, const int memory_limit_mb) {
			this->id = std::move(id);
			this->time_limit_ms = time_limit_ms;
			this->memory_limit_mb = memory_limit_mb;
		}
	};

	static vector<Problem> problem_list;

	inline Problem load_problem(const string& id) {
		std::ifstream file("problems/" + id + "/config.cfg");
		int time_limit_ms = 0, memory_limit_mb = 0, test_cases_count = 0;
		string line;
		while (getline(file, line)) {
			if (line.empty() || line[0] == '#') continue;
			if (const size_t delimiter_pos = line.find('='); delimiter_pos != string::npos) {
				// Parse configuration file
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
		Problem problem(id, time_limit_ms, memory_limit_mb);
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
				const string id = entry.path().filename().string();
				problem_list.push_back(load_problem(id));
				logger->info("Loaded problem ID: {}", id);
			} catch (const std::exception& e) {
				logger->error("Failed to load problem {}: {}", entry.path().filename().string(), e.what());
			}
		}
		logger->info("Indexed {} problems", problem_list.size());
	}

}

#endif
