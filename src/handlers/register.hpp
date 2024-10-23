#ifndef REGISTER_HPP
#define REGISTER_HPP

#include <unordered_map>
#include <string>
#include <functional>

#include "abstract.hpp"
#include "c.hpp"
#include "cpp.hpp"

namespace handlers {

    static std::unordered_map<std::string, std::function<IHandler*(const data::Submission&, const data::Problem&)>> handlers;

    inline void register_handler(const std::string& name, const std::function<IHandler*(const data::Submission&, const data::Problem&)>& handler) {
        handlers[name] = handler;
    }

    inline IHandler* create_handler(const data::Submission& submission, const data::Problem& problem) {
        if (const auto it = handlers.find(submission.target_handler); it != handlers.end()) {
            const auto instance = it->second(submission, problem);
            instance->variant = submission.target_handler;
            return instance;
        }
        throw std::runtime_error("Handler not found: " + submission.target_handler);
    }

    inline void init() {
        for (const auto i : {"c", "c98", "c11", "c17"}) {
            register_handler(i, [](const data::Submission& submission, const data::Problem& problem) -> IHandler* {
                return new C(submission, problem);
            });
        }
        for (const auto i : {"c++", "c++98", "c++11", "c++17"}) {
            register_handler(i, [](const data::Submission& submission, const data::Problem& problem) -> IHandler* {
                return new Cpp(submission, problem);
            });
        }
        logger->info("Registered {} handlers", handlers.size());
    }

}

#endif