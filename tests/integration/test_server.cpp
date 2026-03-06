#include <gtest/gtest.h>
#include "server/http_server.hpp"
#include "utils/logger.hpp"
#include <thread>
#include <chrono>

TEST(ServerIntegrationTest, StartStop) {
    http::utils::Config config;
    config.port = 8089;
    http::utils::Logger logger;
    http::server::HttpServer server(config, logger);
    
    std::thread t([&server]() {
        server.start();
    });
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    server.stop();
    t.join();
    
    SUCCEED();
}
