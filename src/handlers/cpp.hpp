#ifndef CPP_PROFILE_HPP
#define CPP_PROFILE_HPP
#include "abstract.hpp"

namespace handlers {

    class Cpp : public IHandler {
    private:
        const string compile_command = utils::get_env("CPP_COMPILE_COMMAND", "g++");
        const string default_args = "-O3 -static -DONLINE_JUDGE -lm -o main.out";

        void compile() override {
            string standard = variant;

            const string command = compile_command + ' ' + default_args + " main.cpp";
            if (system(command.c_str()) != 0) {
                throw std::runtime_error("Compilation failed");
            }
        }

    public:
        Cpp(data::Submission submission, data::Problem problem) : IHandler(std::move(submission), std::move(problem)) {}

        vector<string> get_variants() override {
            return {"default", "c++98", "c++11", "c++17"};
        }
    };

}

#endif
