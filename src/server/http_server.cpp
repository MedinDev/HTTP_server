#include "server/http_server.hpp"
#include "server/connection.hpp"
#include "server/request.hpp"
#include "server/response.hpp"
#include "utils/logger.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include <iostream>

namespace http {
namespace server {

HttpServer::HttpServer(const utils::Config& config)
    : config_(config), thread_pool_(config.thread_pool_size), running_(false)
{
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ == -1) {
        throw std::runtime_error("Failed to create socket");
    }

    int opt = 1;
    if (setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        throw std::runtime_error("setsockopt failed");
    }

    address_.sin_family = AF_INET;
    address_.sin_addr.s_addr = INADDR_ANY;
    address_.sin_port = htons(config.port);

    if (bind(server_socket_, (struct sockaddr*)&address_, sizeof(address_)) < 0) {
        throw std::runtime_error("Bind failed");
    }
}

HttpServer::~HttpServer() {
    stop();
}

void HttpServer::start() {
    if (listen(server_socket_, 3) < 0) {
        throw std::runtime_error("Listen failed");
    }

    running_ = true;
    utils::Logger::log(utils::LogLevel::INFO, "Server started on port " + std::to_string(config_.port));
    
    accept_connections();
}

void HttpServer::stop() {
    running_ = false;
    if (server_socket_ != -1) {
        close(server_socket_);
        server_socket_ = -1;
    }
}

void HttpServer::accept_connections() {
    int addrlen = sizeof(address_);
    while (running_) {
        int new_socket = accept(server_socket_, (struct sockaddr*)&address_, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            if (running_) {
                utils::Logger::log(utils::LogLevel::ERROR, "Accept failed");
            }
            continue;
        }
        
        thread_pool_.enqueue([this, new_socket] {
            this->handle_client(new_socket);
        });
    }
}

void HttpServer::handle_client(int client_socket) {
    Connection conn(client_socket);
    char buffer[1024] = {0};
    int valread = read(client_socket, buffer, 1024);
    
    if (valread > 0) {
        Request req = Request::parse(std::string(buffer, valread));
        Response res;
        
        router_.route(req, res);
        
        std::string response_str = res.to_string();
        send(client_socket, response_str.c_str(), response_str.length(), 0);
    }
}

}
}
