#pragma once
#include <string>
#include <memory>
#include <netinet/in.h>
#include "core/thread_pool.hpp"
#include "server/router.hpp"
#include "utils/config.hpp"

namespace http {
namespace server {

class HttpServer {
public:
    HttpServer(const utils::Config& config);
    ~HttpServer();

    void start();
    void stop();

    Router& get_router() { return router_; }

private:
    utils::Config config_;
    core::ThreadPool thread_pool_;
    Router router_;
    int server_socket_;
    bool running_;
    struct sockaddr_in address_;

    void accept_connections();
    void handle_client(int client_socket);
};

}
}
