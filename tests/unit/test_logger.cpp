#include <gtest/gtest.h>
#include "utils/logger.hpp"

TEST(LoggerTest, BasicLogging) {
    // This is just a compilation and basic runtime check
    // In a real scenario, we might redirect stdout/stderr to verify output
    http::utils::Logger::log(http::utils::LogLevel::INFO, "Test Info Message");
    http::utils::Logger::log(http::utils::LogLevel::ERROR, "Test Error Message");
    SUCCEED();
}
