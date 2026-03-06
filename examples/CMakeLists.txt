add_executable(simple_api simple_api/main.cpp)
target_link_libraries(simple_api PRIVATE server_lib)

add_executable(static_server static_server/main.cpp)
target_link_libraries(static_server PRIVATE server_lib)
