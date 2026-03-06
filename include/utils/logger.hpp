#pragma once

#include <string>
#include <mutex>
#include <atomic>

namespace http {
namespace utils {

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    static void log(LogLevel level, const std::string& message);
    static void set_level(LogLevel level);

private:
    static std::mutex mutex_;
    static std::atomic<LogLevel> current_level_;
};

} // namespace utils
} // namespace http
