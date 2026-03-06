#include <gtest/gtest.h>
#include "server/request.hpp"

TEST(RequestTest, ParseSimpleGet) {
    std::string raw = "GET /index.html HTTP/1.1\r\nHost: localhost:8080\r\n\r\n";
    auto req = http::server::Request::parse(raw);
    
    EXPECT_EQ(req.method, "GET");
    EXPECT_EQ(req.path, "/index.html");
    EXPECT_EQ(req.version, "HTTP/1.1");
    EXPECT_EQ(req.headers["Host"], "localhost:8080");
}

TEST(RequestTest, ParsePostWithBody) {
    std::string raw = "POST /api/data HTTP/1.1\r\nContent-Length: 10\r\n\r\nHelloWorld";
    auto req = http::server::Request::parse(raw);
    
    EXPECT_EQ(req.method, "POST");
    EXPECT_EQ(req.body, "HelloWorld");
}
