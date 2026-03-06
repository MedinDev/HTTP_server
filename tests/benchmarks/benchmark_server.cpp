#include "server/http_server.hpp"
#include <iostream>
#include <chrono>

int main() {
    http::utils::Config config;
    config.port = 8080;
    http::server::HttpServer server(config);

    server.get_router().add_route("GET", "/", [](const http::server::Request&, http::server::Response& res) {
        res.body = "Benchmark";
    });

    std::cout << "Starting Benchmark Server on 8080" << std::endl;
    server.start();
    return 0;
}
