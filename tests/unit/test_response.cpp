#include <gtest/gtest.h>
#include "server/response.hpp"

TEST(ResponseTest, ToString) {
    http::server::Response res;
    res.status_code = 200;
    res.status_message = "OK";
    res.body = "Hello";
    res.headers["Content-Type"] = "text/plain";
    
    std::string raw = res.to_string();
    EXPECT_NE(raw.find("HTTP/1.1 200 OK"), std::string::npos);
    EXPECT_NE(raw.find("Content-Type: text/plain"), std::string::npos);
    EXPECT_NE(raw.find("Content-Length: 5"), std::string::npos);
    EXPECT_NE(raw.find("\r\n\r\nHello"), std::string::npos);
}
