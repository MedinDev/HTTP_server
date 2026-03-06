#include "server/http_server.hpp"
#include <thread>
#include <chrono>
#include <iostream>

void test_server_start_stop() {
    http::utils::Config config;
    config.port = 8089;
    http::server::HttpServer server(config);
    
    std::thread t([&server]() {
        server.start();
    });
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    server.stop();
    t.join();
    
    std::cout << "test_server_start_stop passed" << std::endl;
}

int main() {
    test_server_start_stop();
    return 0;
}
