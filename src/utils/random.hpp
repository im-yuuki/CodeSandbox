#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>
#include <string>

constexpr char CHARSET[] = "abcdefghijklmnopqrstuvwxyz0123456789";

namespace utils {

	inline std::string random_dir_name() {
		// Generate a random 32-character string, which can be used as a temporary directory name
		using namespace std;
		char result[33];
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dis(0, sizeof(CHARSET) - 2);
		for (int i = 0; i < 32; i++) result[i] = CHARSET[dis(gen)];
		result[32] = '\0';
		return result;
	}

}

#endif
