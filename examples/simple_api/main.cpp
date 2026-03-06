#include "server/http_server.hpp"
#include <iostream>

int main() {
    try {
        http::utils::Config config;
        config.port = 8081;
        http::server::HttpServer server(config);

        server.get_router().add_route("GET", "/api/data", [](const http::server::Request&, http::server::Response& res) {
            res.body = "{\"data\": [1, 2, 3]}";
            res.headers["Content-Type"] = "application/json";
        });

        std::cout << "Starting API server on port 8081..." << std::endl;
        server.start();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
