#ifndef PROBLEMS_HPP
#define PROBLEMS_HPP

#include <fstream>
#include <string>

namespace data {

	struct TestCase {
		std::string input, output;
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
		std::string id;
		int time_limit_secs, memory_limit_mb;
		std::vector<TestCase> test_cases;

		Problem(std::string id, const int time_limit_secs, const int memory_limit_mb) {
			this->id = std::move(id);
			this->time_limit_secs = time_limit_secs;
			this->memory_limit_mb = memory_limit_mb;
		}
	};

}

#endif
