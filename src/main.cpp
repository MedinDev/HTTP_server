#include "server/http_server.hpp"
#include "handlers/route_handler.hpp"
#include "utils/logger.hpp"
#include <iostream>
#include <csignal>

namespace {
    std::function<void(int)> shutdown_handler;
    void signal_handler(int signal) { shutdown_handler(signal); }
}

int main() {
    try {
        http::utils::Config config;
        config.port = 8080;
        config.thread_pool_size = 4;

        http::server::HttpServer server(config);

        // Define routes
        server.get_router().add_route("GET", "/", [](const http::server::Request& req, http::server::Response& res) {
            res.body = "Welcome to the HTTP Server!";
        });

        server.get_router().add_route("GET", "/api/hello", [](const http::server::Request& req, http::server::Response& res) {
            res.body = "{\"message\": \"Hello, World!\"}";
            res.headers["Content-Type"] = "application/json";
        });

        // Setup signal handling for graceful shutdown
        shutdown_handler = [&](int signal) {
            http::utils::Logger::log(http::utils::LogLevel::INFO, "Shutting down server...");
            server.stop();
        };
        std::signal(SIGINT, signal_handler);
        std::signal(SIGTERM, signal_handler);

        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
