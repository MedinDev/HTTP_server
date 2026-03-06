#include "utils/logger.hpp"
#include <iostream>

namespace http {
namespace utils {

std::mutex Logger::mutex_;
std::atomic<LogLevel> Logger::current_level_{LogLevel::INFO};

void Logger::set_level(LogLevel level) {
    current_level_ = level;
}

void Logger::log(LogLevel level, const std::string& message) {
    if (level < current_level_) return;

    std::lock_guard<std::mutex> lock(mutex_);
    switch (level) {
        case LogLevel::DEBUG:
            std::cout << "[DEBUG] ";
            std::cout << message << std::endl;
            break;
        case LogLevel::INFO:
            std::cout << "[INFO] ";
            std::cout << message << std::endl;
            break;
        case LogLevel::WARNING:
            std::cerr << "[WARNING] ";
            std::cerr << message << std::endl;
            break;
        case LogLevel::ERROR:
            std::cerr << "[ERROR] ";
            std::cerr << message << std::endl;
            break;
    }
}

} // namespace utils
} // namespace http
