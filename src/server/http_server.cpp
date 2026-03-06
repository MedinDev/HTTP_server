#include "server/http_server.hpp"
#include <iostream>

namespace http {
namespace server {

HttpServer::HttpServer(const utils::Config& config, utils::Logger& logger)
    : io_context_(),
      acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), config.port)),
      thread_pool_(config.thread_pool_size),
      logger_(logger),
      running_(false)
{
    logger_.log(utils::LogLevel::INFO, "Initializing HttpServer on port " + std::to_string(config.port));
}

HttpServer::~HttpServer() {
    stop();
}

void HttpServer::start() {
    if (running_) return;
    running_ = true;
    start_time_ = std::chrono::steady_clock::now();
    
    logger_.log(utils::LogLevel::INFO, "Starting server...");
    
    accept_connections();
    
    // Run io_context in the main thread (blocking)
    io_context_.run();
}

void HttpServer::stop() {
    if (!running_) return;
    running_ = false;
    logger_.log(utils::LogLevel::INFO, "Stopping server...");
    
    acceptor_.close();
    io_context_.stop();
}

void HttpServer::accept_connections() {
    auto connection = std::make_shared<Connection>(
        boost::asio::ip::tcp::socket(io_context_), router_, thread_pool_, metrics_
    );
    
    acceptor_.async_accept(connection->socket(),
        [this, connection](const boost::system::error_code& error) {
            handle_accept(connection, error);
        });
}

void HttpServer::handle_accept(std::shared_ptr<Connection> connection, const boost::system::error_code& error) {
    if (!error) {
        metrics_.active_connections++;
        connection->start();
    } else {
        logger_.log(utils::LogLevel::ERROR, "Accept error: " + error.message());
    }
    
    if (running_) {
        accept_connections();
    }
}

} // namespace server
} // namespace http
