#ifndef ENV_HPP
#define ENV_HPP

#include <string>
#include <fstream>
#include <map>
#include <cstdlib>

using namespace std;

namespace utils {
    static map<string, string> env_vars;

    inline string trim(const string& str) {
        const size_t first = str.find_first_not_of(' ');
        if (first == string::npos) return "";
        const size_t last = str.find_last_not_of(' ');
        return str.substr(first, last - first + 1);
    }

    inline void load_env(const string& env_file = ".env") {
        ifstream file(env_file);
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            if (const size_t delimiter_pos = line.find('='); delimiter_pos != string::npos) {
                string key = trim(line.substr(0, delimiter_pos));
                string value = trim(line.substr(delimiter_pos + 1));
                if (!value.empty() && value.front() == '"' && value.back() == '"') {
                    value = value.substr(1, value.size() - 2);
                }
                env_vars[key] = value;
            }
        }
        file.close();
    }

    inline string get_env(const string& key, const string& default_value = "") {
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
