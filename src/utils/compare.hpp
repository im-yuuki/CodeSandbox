#ifndef COMPARE_HPP
#define COMPARE_HPP

#include <sstream>
using namespace std;

namespace utils {

	inline bool token_compare(std::stringstream& ss, const std::string& sample) {
		istringstream sample_stream(sample);
		string token1, token2;
		while (ss >> token1 && sample_stream >> token2) if (token1 != token2) {
			return false; // Token mismatch
		}
		return !(ss >> token1 || sample_stream >> token2); // Ensure that both streams are empty
	}

}

#endif
