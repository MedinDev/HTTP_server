cmake_minimum_required(VERSION 3.20)
project(HighPerformanceHttpServer VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build types
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Profile;Benchmark")

# Profile build flags
set(CMAKE_CXX_FLAGS_PROFILE "-O2 -pg -g -DNDEBUG")
set(CMAKE_EXE_LINKER_FLAGS_PROFILE "-pg")
set(CMAKE_SHARED_LINKER_FLAGS_PROFILE "-pg")

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find packages
find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

# Include directories
include_directories(${Boost_INCLUDE_DIRS})
include_directories(include)

# Server Source Files
set(SERVER_SOURCES
    src/server/http_server.cpp
    src/server/connection.cpp
    src/server/request.cpp
    src/server/response.cpp
    src/server/router.cpp
    src/core/thread_pool.cpp
    src/core/async_worker.cpp
    src/utils/logger.cpp
    src/utils/config.cpp
    src/utils/buffer.cpp
    src/handlers/static_file_handler.cpp
    src/handlers/route_handler.cpp
)

# Create a library for testing and reuse
add_library(server_lib STATIC ${SERVER_SOURCES})
target_link_libraries(server_lib 
    ${Boost_LIBRARIES} 
    ${CMAKE_THREAD_LIBS_INIT}
)

# Main executable
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} server_lib)

# Tests
enable_testing()
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

# Installation
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
