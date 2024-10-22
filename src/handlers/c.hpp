#ifndef C17_PROFILE_HPP
#define C17_PROFILE_HPP

#include "abstract.hpp"
#include "../utils/env.hpp"

namespace handlers {

	class C : public IHandler {
	private:
		const string compile_command = utils::get_env(
			"C17_COMPILE_COMMAND",
			"gcc"
			);

		const string default_args = "-O3 -static -DONLINE_JUDGE -lm -o main.out";

		void compile() override {
			const string command = compile_command + ' ' + default_args + " main.cpp";
			if (system(command.c_str()) != 0) {
				throw std::runtime_error("Compilation failed");
			}
		}

	public:
		C(data::Submission submission, data::Problem problem) : IHandler(std::move(submission), std::move(problem)) {}

		vector<string> get_variants() override {
			return {"default"};
		}
	};

}

#endif