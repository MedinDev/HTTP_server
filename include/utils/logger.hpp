#pragma once

#include <string>
#include <mutex>

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

private:
    static std::mutex mutex_;
};

} // namespace utils
} // namespace http
