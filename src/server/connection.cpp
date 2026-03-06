#include "server/connection.hpp"
#include "utils/logger.hpp"
#include <iostream>
#include <string>

namespace http {
namespace server {

Connection::Connection(boost::asio::ip::tcp::socket socket, Router& router, core::ThreadPool& thread_pool, Metrics& metrics)
    : socket_(std::move(socket)), router_(router), thread_pool_(thread_pool), metrics_(metrics) {}

void Connection::start() {
    do_read();
}

void Connection::do_read() {
    auto self = shared_from_this();
    socket_.async_read_some(boost::asio::buffer(buffer_),
        [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
            if (!ec) {
                metrics_.total_requests++;
                std::string raw_request(buffer_.data(), bytes_transferred);
                try {
                    request_ = Request::parse(raw_request);
                    process_request();
                } catch (const std::exception& e) {
                    response_.status_code = 400;
                    response_.status_message = "Bad Request";
                    response_.body = "Bad Request";
                    do_write();
                }
            }
        });
}

void Connection::process_request() {
    auto self = shared_from_this();
    
    thread_pool_.enqueue([this, self]() {
        try {
            auto response_opt = router_.route(request_);
            if (response_opt) {
                response_ = *response_opt;
            } else {
                response_.status_code = 404;
                response_.status_message = "Not Found";
                response_.body = "404 Not Found";
            }
            
            boost::asio::post(socket_.get_executor(), [this, self]() {
                do_write();
            });
        } catch (const std::exception& e) {
            response_.status_code = 500;
            response_.status_message = "Internal Server Error";
            response_.body = "Internal Server Error";
            boost::asio::post(socket_.get_executor(), [this, self]() {
                do_write();
            });
        }
    });
}

void Connection::do_write() {
    auto self = shared_from_this();
    auto response_str = std::make_shared<std::string>(response_.to_string());
    
    boost::asio::async_write(socket_, boost::asio::buffer(*response_str),
        [this, self, response_str](boost::system::error_code ec, std::size_t) {
            if (!ec) {
                boost::system::error_code ignored_ec;
                socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
            }
        });
}

} // namespace server
} // namespace http
