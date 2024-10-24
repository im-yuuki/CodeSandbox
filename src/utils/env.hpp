#ifndef ENV_HPP
#define ENV_HPP

#include <string>
#include <fstream>
#include <unordered_map>
#include <cstdlib>

namespace utils {
    static std::unordered_map<std::string, std::string> env_vars;

    inline std::string trim(const std::string& str) {
        // Remove leading and trailing spaces
        const size_t first = str.find_first_not_of(' ');
        if (first == std::string::npos) return "";
        const size_t last = str.find_last_not_of(' ');
        return str.substr(first, last - first + 1);
    }

    inline void load_env() {
        // Parse the environment file
        std::ifstream file(".env");
        if (!file.is_open()) return;
        std::string line;
        while (getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            if (const size_t delimiter_pos = line.find('='); delimiter_pos != std::string::npos) {
                std::string key = trim(line.substr(0, delimiter_pos));
                std::string value = trim(line.substr(delimiter_pos + 1));
                if (!value.empty() && value.front() == '"' && value.back() == '"') {
                    value = value.substr(1, value.size() - 2);
                }
                env_vars[key] = value;
            }
        }
        file.close();
    }

    inline std::string get_env(const std::string& key, const std::string& default_value = "") {
        // Get the value of an environment variable, or return the default value if it doesn't exist
        if (env_vars.contains(key)) {
            return env_vars[key];
        }
        if (const char* env_value = std::getenv(key.c_str()); env_value != nullptr) {
            return { env_value };
        }
        return default_value;
    }
}

#endif
