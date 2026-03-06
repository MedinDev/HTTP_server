#include "server/response.hpp"
#include <cassert>
#include <iostream>

void test_response_to_string() {
    http::server::Response res;
    res.status_code = 200;
    res.status_message = "OK";
    res.body = "Hello";
    res.headers["Content-Type"] = "text/plain";
    
    std::string raw = res.to_string();
    assert(raw.find("HTTP/1.1 200 OK") != std::string::npos);
    assert(raw.find("Content-Type: text/plain") != std::string::npos);
    assert(raw.find("Content-Length: 5") != std::string::npos);
    assert(raw.find("\r\n\r\nHello") != std::string::npos);
    
    std::cout << "test_response_to_string passed" << std::endl;
}

int main() {
    test_response_to_string();
    return 0;
}
