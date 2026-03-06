#include "server/router.hpp"
#include <cassert>
#include <iostream>

void test_simple_route() {
    http::server::Router router;
    bool called = false;
    
    router.add_route("GET", "/", [&](const http::server::Request&, http::server::Response&) {
        called = true;
    });
    
    http::server::Request req;
    req.method = "GET";
    req.path = "/";
    http::server::Response res;
    
    router.route(req, res);
    assert(called);
    
    std::cout << "test_simple_route passed" << std::endl;
}

void test_not_found() {
    http::server::Router router;
    http::server::Request req;
    req.method = "GET";
    req.path = "/unknown";
    http::server::Response res;
    
    router.route(req, res);
    assert(res.status_code == 404);
    
    std::cout << "test_not_found passed" << std::endl;
}

int main() {
    test_simple_route();
    test_not_found();
    return 0;
}
