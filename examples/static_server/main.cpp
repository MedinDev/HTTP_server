#include "server/http_server.hpp"
#include "handlers/static_file_handler.hpp"
#include <iostream>

int main() {
    try {
        http::utils::Config config;
        config.port = 8082;
        http::server::HttpServer server(config);

        http::handlers::StaticFileHandler static_handler("www");
        
        // Wildcard route simulation for static files would need better router support
        // For now, we manually map specific files
        server.get_router().add_route("GET", "/index.html", static_handler);
        server.get_router().add_route("GET", "/", static_handler);

        std::cout << "Starting Static server on port 8082..." << std::endl;
        server.start();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
