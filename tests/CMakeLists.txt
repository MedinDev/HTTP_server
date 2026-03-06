# Unit Tests

# Use system installed GTest via Brew
find_package(GTest REQUIRED)

enable_testing()

# Consolidated Unit Tests
add_executable(unit_tests
    unit/test_request.cpp
    unit/test_response.cpp
    unit/test_router.cpp
    unit/test_logger.cpp
)
target_link_libraries(unit_tests PRIVATE server_lib GTest::gtest_main)
add_test(NAME unit_tests COMMAND unit_tests)

# Consolidated Integration Tests
add_executable(integration_tests
    integration/test_server.cpp
    integration/test_connections.cpp
)
target_link_libraries(integration_tests PRIVATE server_lib GTest::gtest_main)
add_test(NAME integration_tests COMMAND integration_tests)

# Keep individual tests for granular running if needed (optional, but good for development)
# But to avoid target name conflicts, I'll remove them or rename them.
# Given the user request, I'll just stick to the consolidated ones to keep it clean and matching the README.

# Benchmarks
add_executable(benchmark_server benchmarks/benchmark_server.cpp)
target_link_libraries(benchmark_server PRIVATE server_lib)
