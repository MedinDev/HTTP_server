#include <gtest/gtest.h>
#include "server/router.hpp"

class RouterTest : public ::testing::Test {
protected:
    void SetUp() override {
        router_.get("/test", [](const http::server::Request& req) {
            http::server::Response res;
            res.body = "OK";
            res.status_code = 200;
            return res;
        });
        
        router_.get("/users/:id", [](const http::server::Request& req) {
            http::server::Response res;
            if (req.path_params.count("id")) {
                res.body = "User " + req.path_params.at("id");
            }
            res.status_code = 200;
            return res;
        });
    }
    
    http::server::Router router_;
};

TEST_F(RouterTest, SimpleRoute) {
    http::server::Request req;
    req.method = "GET";
    req.path = "/test";
    
    auto res = router_.route(req);
    ASSERT_TRUE(res.has_value());
    EXPECT_EQ(res->status_code, 200);
    EXPECT_EQ(res->body, "OK");
}

TEST_F(RouterTest, PathParams) {
    http::server::Request req;
    req.method = "GET";
    req.path = "/users/123";
    
    auto res = router_.route(req);
    ASSERT_TRUE(res.has_value());
    EXPECT_EQ(res->body, "User 123");
}

TEST_F(RouterTest, RouteNotFound) {
    http::server::Request req;
    req.method = "GET";
    req.path = "/nonexistent";
    
    auto res = router_.route(req);
    EXPECT_FALSE(res.has_value());
}
