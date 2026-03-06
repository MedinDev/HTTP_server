#include <gtest/gtest.h>
#include "server/http_server.hpp"
#include "utils/logger.hpp"
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

TEST(ServerIntegrationTest, ConnectionTest) {
    http::utils::Config config;
    config.port = 8090;
    http::utils::Logger logger;
    http::server::HttpServer server(config, logger);
    
    std::thread t([&server]() {
        server.start();
    });
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8090);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    
    int connect_res = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    ASSERT_GE(connect_res, 0);
    
    std::string req = "GET / HTTP/1.1\r\n\r\n";
    send(sock, req.c_str(), req.length(), 0);
    
    char buffer[1024] = {0};
    read(sock, buffer, 1024);
    std::string res(buffer);
    // Since we didn't add route for /, it should be 404
    EXPECT_NE(res.find("404 Not Found"), std::string::npos);
    
    close(sock);
    server.stop();
    t.join();
}
