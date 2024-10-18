#ifndef C17_PROFILE_HPP
#define C17_PROFILE_HPP

#include "abstract.hpp"
#include "../utils/env.hpp"

namespace profile {

	class C17 : public IProfile {
	private:
		const string compile_command = utils::get_env(
			"C17_COMPILE_COMMAND",
			"gcc -O3 -std=c17 -static -o main.out"
			);

		void compile() {
			const string command = compile_command + " main.cpp";
			if (system(command.c_str()) != 0) {
				throw std::runtime_error("Compilation failed");
			}
		}

	public:
		C17(Submission submission, Problem problem) : IProfile(std::move(submission), std::move(problem)) {}
	};

}

#endif