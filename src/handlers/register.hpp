#ifndef REGISTER_HPP
#define REGISTER_HPP

#include <unordered_map>
#include <string>
#include <functional>

#include "abstract.hpp"

namespace handlers {

    static std::unordered_map<std::string, std::function<IHandler*(const data::Submission&, const data::Problem&)>> handlers;

    inline void register_handler(const std::string& name, const std::function<IHandler*(const data::Submission&, const data::Problem&)>& handler) {
        handlers[name] = handler;
    }

    inline IHandler& create_handler(const std::string& name, const data::Submission& submission, const data::Problem& problem) {
        if (const auto it = handlers.find(name); it != handlers.end()) {
            const auto instance = it->second(submission, problem);
            instance->set_variant(name);
            return *instance;
        }
        throw std::runtime_error("Variant not found");
    }

}

#endif