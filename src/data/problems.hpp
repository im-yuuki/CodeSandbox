#ifndef PROBLEMS_HPP
#define PROBLEMS_HPP

#include <string>
#include <utility>
using namespace std;

struct TestCase {
	string input;
	string output;
};

struct Problem {
	string id;
	int time_limit_ms = 1000;
	int memory_limit_mb = 1024;
	int test_case_count = 0;
	TestCase* test_cases;

	Problem(string id, const int time_limit_ms, const int memory_limit_mb, const int test_case_count, TestCase* test_cases) {
		this->id = std::move(id);
		this->time_limit_ms = time_limit_ms;
		this->memory_limit_mb = memory_limit_mb;
		this->test_case_count = test_case_count;
		this->test_cases = test_cases;
	}

	static Problem load_from_disk(const string& id) {
		// Load problem from disk
		return Problem("1", 1000, 1024, 2, new TestCase[2]);
	}
};


#endif
