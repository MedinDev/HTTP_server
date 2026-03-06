#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <atomic>
#include <chrono>
#include "server/router.hpp"
#include "core/thread_pool.hpp"
#include "utils/logger.hpp"
#include "utils/config.hpp"
#include "server/connection.hpp"
#include "server/metrics.hpp"

namespace http {
namespace server {

class HttpServer {
public:
    HttpServer(const utils::Config& config, utils::Logger& logger);
    ~HttpServer();

    // Delete copy/move constructors
    HttpServer(const HttpServer&) = delete;
    HttpServer& operator=(const HttpServer&) = delete;

    // Server lifecycle
    void start();
    void stop();
    bool is_running() const { return running_; }

    // Router configuration
    Router& router() { return router_; }

    // Performance metrics
    const Metrics& get_metrics() const { return metrics_; }

private:
    void accept_connections();
    void handle_accept(std::shared_ptr<Connection> connection, 
                      const boost::system::error_code& error);
    
    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    
    Router router_;
    core::ThreadPool thread_pool_;
    utils::Logger& logger_;
    
    std::atomic<bool> running_;
    Metrics metrics_;
    std::chrono::steady_clock::time_point start_time_;
};

} // namespace server
} // namespace http
