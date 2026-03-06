# Unit Tests
add_executable(test_request unit/test_request.cpp)
target_link_libraries(test_request PRIVATE server_lib)
add_test(NAME test_request COMMAND test_request)

add_executable(test_response unit/test_response.cpp)
target_link_libraries(test_response PRIVATE server_lib)
add_test(NAME test_response COMMAND test_response)

add_executable(test_router unit/test_router.cpp)
target_link_libraries(test_router PRIVATE server_lib)
add_test(NAME test_router COMMAND test_router)

add_executable(test_logger unit/test_logger.cpp)
target_link_libraries(test_logger PRIVATE server_lib)
add_test(NAME test_logger COMMAND test_logger)

# Integration Tests
add_executable(test_server integration/test_server.cpp)
target_link_libraries(test_server PRIVATE server_lib)
add_test(NAME test_server COMMAND test_server)

add_executable(test_connections integration/test_connections.cpp)
target_link_libraries(test_connections PRIVATE server_lib)
add_test(NAME test_connections COMMAND test_connections)

# Benchmarks
add_executable(benchmark_server benchmarks/benchmark_server.cpp)
target_link_libraries(benchmark_server PRIVATE server_lib)
