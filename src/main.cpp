#include "server/http_server.hpp"
#include "handlers/route_handler.hpp"
#include "utils/logger.hpp"
#include "utils/config.hpp"
#include <iostream>
#include <csignal>
#include <string>
#include <thread>
#include <chrono>
#include <iomanip>

namespace {
    std::function<void(int)> shutdown_handler;
    void signal_handler(int signal) { 
        if (shutdown_handler) shutdown_handler(signal); 
    }

    void print_usage(const char* prog_name) {
        std::cerr << "Usage: " << prog_name << " [-c config_file] [--metrics]" << std::endl;
        std::cerr << "Options:" << std::endl;
        std::cerr << "  -c <file>   Path to configuration file" << std::endl;
        std::cerr << "  --metrics   Enable real-time metrics display (blocks stdout)" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    try {
        http::utils::Config config;
        std::string config_path;
        bool show_metrics = false;

        // Parse command line arguments
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "-c") {
                if (i + 1 < argc) {
                    config_path = argv[++i];
                } else {
                    print_usage(argv[0]);
                    return 1;
                }
            } else if (arg == "--metrics") {
                show_metrics = true;
            } else if (arg == "-h" || arg == "--help") {
                print_usage(argv[0]);
                return 0;
            }
        }

        // Load config if provided
        if (!config_path.empty()) {
            config.load(config_path);
        }

        http::utils::Logger logger;
        
        // Set log level from config
        if (config.log_level == "DEBUG") logger.set_level(http::utils::LogLevel::DEBUG);
        else if (config.log_level == "INFO") logger.set_level(http::utils::LogLevel::INFO);
        else if (config.log_level == "WARN" || config.log_level == "WARNING") logger.set_level(http::utils::LogLevel::WARNING);
        else if (config.log_level == "ERROR") logger.set_level(http::utils::LogLevel::ERROR);

        http::server::HttpServer server(config, logger);

        // Define routes
        server.router().get("/api/hello", [](const http::server::Request& req) -> http::server::Response {
            return http::server::Response::json("{\"message\": \"Hello, World!\"}");
        });

        // Example of path parameters
        server.router().get("/users/:id", [](const http::server::Request& req) -> http::server::Response {
            std::string user_id = "unknown";
            if (req.path_params.count("id")) {
                user_id = req.path_params.at("id");
            }
            return http::server::Response::json("{\"user_id\": \"" + user_id + "\"}");
        });

        // Benchmark route
        server.router().get("/api/users", [](const http::server::Request& req) -> http::server::Response {
            // Mock users data
            std::string users_json = "[{\"id\":1,\"name\":\"Alice\"},{\"id\":2,\"name\":\"Bob\"},{\"id\":3,\"name\":\"Charlie\"}]";
            return http::server::Response::json(users_json);
        });
        
        // API Metrics endpoint for the dashboard
        server.router().get("/api/metrics", [&server](const http::server::Request& req) -> http::server::Response {
            const auto& metrics = server.get_metrics();
            auto now = std::chrono::steady_clock::now();
            // We need to access start_time_ but it's private. 
            // Let's assume uptime is tracked in metrics or just return what we have.
            
            std::string json = "{";
            json += "\"total_requests\":" + std::to_string(metrics.total_requests.load()) + ",";
            json += "\"active_connections\":" + std::to_string(metrics.active_connections.load()) + ",";
            json += "\"bytes_sent\":" + std::to_string(metrics.bytes_sent.load());
            json += "}";
            
            return http::server::Response::json(json);
        });

        // Example of static file serving - MUST be last to avoid shadowing other routes
        server.router().serve_static("/", config.static_dir);
        server.router().serve_static("/static", config.static_dir);

        // Setup signal handling for graceful shutdown
        shutdown_handler = [&](int signal) {
            if (!show_metrics) {
                http::utils::Logger::log(http::utils::LogLevel::INFO, "Shutting down server...");
            }
            server.stop();
        };
        std::signal(SIGINT, signal_handler);
        std::signal(SIGTERM, signal_handler);

        if (show_metrics) {
            // Run server in a separate thread so we can print metrics in main thread
            std::thread server_thread([&server]() {
                server.start();
            });

            // Disable logger output to stdout to avoid messing up metrics display
            // This is a simple hack; a better way would be to have separate sinks
            logger.set_level(http::utils::LogLevel::ERROR); 

            std::cout << "Server running on port " << config.port << " with metrics enabled." << std::endl;
            std::cout << "Press Ctrl+C to stop." << std::endl;
            
            auto last_time = std::chrono::steady_clock::now();
            uint64_t last_requests = 0;

            // Give server a moment to start
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            while (server.is_running()) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                
                auto now = std::chrono::steady_clock::now();
                const auto& metrics = server.get_metrics();
                
                double elapsed = std::chrono::duration<double>(now - last_time).count();
                uint64_t current_requests = metrics.total_requests;
                double req_per_sec = (current_requests - last_requests) / elapsed;
                
                std::cout << "\rRequests/sec: " << std::fixed << std::setprecision(1) << req_per_sec 
                          << " | Active: " << metrics.active_connections 
                          << " | Total: " << metrics.total_requests << "     " << std::flush;
                
                last_time = now;
                last_requests = current_requests;
            }
            std::cout << std::endl;
            
            if (server_thread.joinable()) {
                server_thread.join();
            }
        } else {
            // Run server in main thread (blocking)
            server.start();
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
