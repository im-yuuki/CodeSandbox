#ifndef REGISTER_HPP
#define REGISTER_HPP

#include <unordered_map>
#include <string>
#include <functional>

#include "abstract.hpp"
#include "c.hpp"
#include "cpp.hpp"
#include "rust.hpp"
#include "python.hpp"

namespace modules {

    static std::unordered_map<std::string, std::function<IModules*(const data::Submission*, const data::Problem*)>> modules;

    inline void register_handler(const std::string& name, const std::function<IModules*(const data::Submission*, const data::Problem*)>& module) {
        modules[name] = module;
    }

    inline IModules* create_handler(const data::Submission* submission, const data::Problem* problem) {
        if (const auto it = modules.find(submission->target_module); it != modules.end()) {
            const auto instance = it->second(submission, problem);
            instance->variant = submission->target_module;
            return instance;
        }
        throw std::runtime_error("Handler not found: " + submission->target_module);
    }

    inline void init() {
        for (const auto i : {"c", "c98", "c11", "c17"}) {
            register_handler(i, [](const data::Submission* submission, const data::Problem* problem) -> IModules* {
                return new C(submission, problem);
            });
        }
        for (const auto i : {"c++", "c++98", "c++11", "c++17"}) {
            register_handler(i, [](const data::Submission* submission, const data::Problem* problem) -> IModules* {
                return new Cpp(submission, problem);
            });
        }
        for (const auto i : {"rust", "rust182"}) {
            register_handler(i, [](const data::Submission* submission, const data::Problem* problem) -> IModules* {
                return new Rust(submission, problem);
            });
        }
        for (const auto i : {"python", "python313"}) {
            register_handler(i, [](const data::Submission* submission, const data::Problem* problem) -> IModules* {
                return new Python(submission, problem);
            });
        }
        logger->info("Registered {} handlers", modules.size());
    }

}

#endif
