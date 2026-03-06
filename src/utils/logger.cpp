#include "utils/logger.hpp"
#include <iostream>

namespace http {
namespace utils {

std::mutex Logger::mutex_;

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    switch (level) {
        case LogLevel::DEBUG:
            std::cout << "[DEBUG] ";
            break;
        case LogLevel::INFO:
            std::cout << "[INFO] ";
            break;
        case LogLevel::WARNING:
            std::cerr << "[WARNING] ";
            break;
        case LogLevel::ERROR:
            std::cerr << "[ERROR] ";
            break;
    }
    std::cout << message << std::endl;
}

} // namespace utils
} // namespace http
