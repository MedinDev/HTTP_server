#pragma once
#include <string>
#include <unordered_map>

namespace http {
namespace utils {

struct Config {
    int port = 8080;
    std::string static_dir = "www";
    int thread_pool_size = 4;
    std::string log_level = "INFO";

    // Load configuration from a file
    void load(const std::string& path);
};

}
}
