#pragma once

#include <functional>
#include <unordered_map>
#include <regex>
#include <vector>
#include <optional>
#include "server/request.hpp"
#include "server/response.hpp"

namespace http {
namespace server {

using RouteHandler = std::function<Response(const Request&)>;
using MiddlewareHandler = std::function<Response(Request&, RouteHandler)>;

class Router {
public:
    // HTTP methods
    void get(const std::string& path, RouteHandler handler);
    void post(const std::string& path, RouteHandler handler);
    void put(const std::string& path, RouteHandler handler);
    void del(const std::string& path, RouteHandler handler);
    
    // Route with parameters (e.g., "/users/:id")
    // This is implicitly handled by get/post/etc if the path contains parameters
    // But we can keep explicit method if needed. 
    // The user provided get_with_params, but usually get() should handle both.
    // I will implement get/post to handle regex if special chars are present.
    
    // Static file serving
    void serve_static(const std::string& mount_point, const std::string& directory);
    
    // Middleware support
    void use(MiddlewareHandler middleware);
    
    // Route matching
    // Changed to take Request& to populate path_params without copying
    std::optional<Response> route(Request& req);
    
private:
    struct Route {
        std::string path;
        std::string method;
        RouteHandler handler;
        std::vector<std::string> param_names;
        std::regex pattern;
        bool is_regex = false;
    };
    
    std::vector<Route> routes_;
    std::vector<MiddlewareHandler> middleware_;
    
    bool match_path(const Route& route, const std::string& path, 
                   std::unordered_map<std::string, std::string>& params);
                   
    std::string regex_from_path(const std::string& path, std::vector<std::string>& param_names);
};

} // namespace server
} // namespace http
