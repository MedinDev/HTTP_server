cmake_minimum_required(VERSION 3.14)
project(http_server VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Include directories
include_directories(include)

# Find all source files
file(GLOB_RECURSE SERVER_SOURCES 
    "src/server/*.cpp"
    "src/core/*.cpp"
    "src/utils/*.cpp"
    "src/handlers/*.cpp"
)

# Create a library for the core functionality
add_library(server_lib STATIC ${SERVER_SOURCES})
target_include_directories(server_lib PUBLIC include)

find_package(Threads REQUIRED)
target_link_libraries(server_lib PUBLIC Threads::Threads)

# Main executable
add_executable(http_server src/main.cpp)
target_link_libraries(http_server PRIVATE server_lib)

# Testing
include(CTest)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# Examples
option(BUILD_EXAMPLES "Build examples" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
