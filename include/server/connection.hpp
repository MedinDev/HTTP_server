#pragma once

#include <memory>
#include <boost/asio.hpp>
#include <array>
#include "server/request.hpp"
#include "server/response.hpp"
#include "server/router.hpp"
#include "core/thread_pool.hpp"
#include "server/metrics.hpp"

namespace http {
namespace server {

class Connection : public std::enable_shared_from_this<Connection> {
public:
    explicit Connection(boost::asio::ip::tcp::socket socket, Router& router, core::ThreadPool& thread_pool, Metrics& metrics);

    void start();
    boost::asio::ip::tcp::socket& socket() { return socket_; }

private:
    void do_read();
    void do_write();
    void process_request();

    boost::asio::ip::tcp::socket socket_;
    Router& router_;
    core::ThreadPool& thread_pool_;
    Metrics& metrics_;
    std::array<char, 8192> buffer_;
    Request request_;
    Response response_;
};

} // namespace server
} // namespace http
