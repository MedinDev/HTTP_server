#include "utils/logger.hpp"
#include <iostream>

void test_logger() {
    // This is just a compilation and basic runtime check
    http::utils::Logger::log(http::utils::LogLevel::INFO, "Test Info Message");
    http::utils::Logger::log(http::utils::LogLevel::ERROR, "Test Error Message");
    std::cout << "test_logger passed" << std::endl;
}

int main() {
    test_logger();
    return 0;
}
