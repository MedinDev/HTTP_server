#pragma once
#include <string>

namespace http {
namespace utils {

struct Config {
    int port = 8080;
    std::string static_dir = "www";
    int thread_pool_size = 4;
};

}
}
