#ifndef COMPARE_HPP
#define COMPARE_HPP

#include <sstream>

namespace utils {

	inline bool token_compare(std::stringstream& ss, const std::string& sample) {
		std::istringstream ss2(sample);
		std::string token1, token2;
		while (true) {
			if (!(ss >> token1)) token1 = "";
			if (!(ss2 >> token2)) token2 = "";
			if (token1.empty() && token2.empty()) return true;
			if (token1 != token2) return false;
		}
	}

}

#endif
