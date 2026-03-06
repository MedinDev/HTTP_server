#include "server/request.hpp"
#include <cassert>
#include <iostream>

void test_parse_simple_get() {
    std::string raw = "GET /index.html HTTP/1.1\r\nHost: localhost:8080\r\n\r\n";
    auto req = http::server::Request::parse(raw);
    
    assert(req.method == "GET");
    assert(req.path == "/index.html");
    assert(req.version == "HTTP/1.1");
    assert(req.headers["Host"] == "localhost:8080");
    
    std::cout << "test_parse_simple_get passed" << std::endl;
}

void test_parse_post_with_body() {
    std::string raw = "POST /api/data HTTP/1.1\r\nContent-Length: 10\r\n\r\nHelloWorld";
    auto req = http::server::Request::parse(raw);
    
    assert(req.method == "POST");
    assert(req.body == "HelloWorld");
    
    std::cout << "test_parse_post_with_body passed" << std::endl;
}

int main() {
    test_parse_simple_get();
    test_parse_post_with_body();
    return 0;
}
