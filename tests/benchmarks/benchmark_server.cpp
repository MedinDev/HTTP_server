#include "server/http_server.hpp"
#include "utils/logger.hpp"
#include <iostream>
#include <chrono>

int main() {
    http::utils::Config config;
    config.port = 8080;
    http::utils::Logger logger;
    http::server::HttpServer server(config, logger);

    server.router().get("/", [](const http::server::Request&) -> http::server::Response {
        http::server::Response res;
        res.body = "Benchmark";
        return res;
    });

    std::cout << "Starting Benchmark Server on 8080" << std::endl;
    server.start();
    return 0;
}
