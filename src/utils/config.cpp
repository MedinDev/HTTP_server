#include "utils/config.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

namespace http {
namespace utils {

static std::string trim(const std::string& str) {
    auto first = str.find_first_not_of(" \t\r\n");
    if (std::string::npos == first) {
        return str;
    }
    auto last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

void Config::load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open config file " << path << ". Using defaults." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Remove comments
        size_t comment_pos = line.find('#');
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }

        line = trim(line);
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string key;
        if (std::getline(iss, key, '=')) {
            std::string value;
            if (std::getline(iss, value)) {
                key = trim(key);
                value = trim(value);
                
                if (key == "port") port = std::stoi(value);
                else if (key == "thread_pool_size") thread_pool_size = std::stoi(value);
                else if (key == "static_dir") static_dir = value;
                else if (key == "log_level") log_level = value;
            }
        }
    }
}

}
}
