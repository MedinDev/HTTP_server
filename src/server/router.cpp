#include "server/router.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace http {
namespace server {

namespace {
    std::string get_mime_type(const std::string& path) {
        static const std::unordered_map<std::string, std::string> mime_types = {
            {".html", "text/html"},
            {".css", "text/css"},
            {".js", "application/javascript"},
            {".json", "application/json"},
            {".png", "image/png"},
            {".jpg", "image/jpeg"},
            {".jpeg", "image/jpeg"},
            {".gif", "image/gif"},
            {".svg", "image/svg+xml"},
            {".ico", "image/x-icon"},
            {".txt", "text/plain"}
        };

        auto ext = std::filesystem::path(path).extension().string();
        if (mime_types.count(ext)) {
            return mime_types.at(ext);
        }
        return "application/octet-stream";
    }
}

void Router::get(const std::string& path, RouteHandler handler) {
    std::vector<std::string> params;
    std::string regex_str = regex_from_path(path, params);
    routes_.push_back({path, "GET", handler, params, std::regex(regex_str), !params.empty()});
}

void Router::post(const std::string& path, RouteHandler handler) {
    std::vector<std::string> params;
    std::string regex_str = regex_from_path(path, params);
    routes_.push_back({path, "POST", handler, params, std::regex(regex_str), !params.empty()});
}

void Router::put(const std::string& path, RouteHandler handler) {
    std::vector<std::string> params;
    std::string regex_str = regex_from_path(path, params);
    routes_.push_back({path, "PUT", handler, params, std::regex(regex_str), !params.empty()});
}

void Router::del(const std::string& path, RouteHandler handler) {
    std::vector<std::string> params;
    std::string regex_str = regex_from_path(path, params);
    routes_.push_back({path, "DELETE", handler, params, std::regex(regex_str), !params.empty()});
}

void Router::serve_static(const std::string& mount_point, const std::string& directory) {
    // Handler for static files
    auto handler = [mount_point, directory](const Request& req) -> Response {
        Response res;
        std::string sub_path = req.path.substr(mount_point.length());
        if (sub_path.empty() || sub_path[0] != '/') {
            sub_path = "/" + sub_path;
        }
        
        // Debug logging
        // std::cout << "[DEBUG] Serve static: mount=" << mount_point 
        //           << " dir=" << directory 
        //           << " req=" << req.path 
        //           << " sub=" << sub_path << std::endl;

        // Correct path resolution: directory + sub_path
        // If directory is "www" and sub_path is "/index.html", we want "www/index.html"
        std::filesystem::path fs_path = std::filesystem::path(directory);
        if (sub_path != "/") {
            // Remove leading slash from sub_path to avoid it being treated as absolute by operator/
            fs_path /= sub_path.substr(1);
        } else {
            fs_path /= "index.html";
        }

        // std::cout << "[DEBUG] fs_path=" << fs_path << " exists=" << std::filesystem::exists(fs_path) << std::endl;
        
        // Security check: prevent directory traversal
        // Simplified check (real implementation should resolve paths)
        if (sub_path.find("..") != std::string::npos) {
            res.status_code = 403;
            res.status_message = "Forbidden";
            res.body = "Access Denied";
            return res;
        }

        if (std::filesystem::exists(fs_path) && std::filesystem::is_regular_file(fs_path)) {
            std::ifstream file(fs_path, std::ios::binary);
            if (file) {
                std::stringstream buffer;
                buffer << file.rdbuf();
                res.body = buffer.str();
                res.headers["Content-Type"] = get_mime_type(fs_path.string());
                return res;
            }
        }
        
        res.status_code = 404;
        res.status_message = "Not Found";
        res.body = "File Not Found: " + fs_path.string();
        return res;
    };
    
    // Use regex to match any path under mount_point
    // e.g. /static/.*
    std::string regex_str;
    if (mount_point == "/") {
        regex_str = "^/.*$";
    } else {
        // Ensure we don't double slash if mount_point ends with /
        std::string base = mount_point;
        if (base.back() == '/') base.pop_back();
        regex_str = "^" + base + "(/.*)?$";
    }
    routes_.push_back({mount_point, "GET", handler, {}, std::regex(regex_str), true});
}

void Router::use(MiddlewareHandler middleware) {
    middleware_.push_back(middleware);
}

std::optional<Response> Router::route(Request& req) {
    for (const auto& route : routes_) {
        if (route.method == req.method) {
            std::unordered_map<std::string, std::string> params;
            if (match_path(route, req.path, params)) {
                req.path_params = params;
                
                // Construct handler chain
                RouteHandler current_handler = route.handler;
                
                // Wrap with middleware in reverse order
                for (auto it = middleware_.rbegin(); it != middleware_.rend(); ++it) {
                    auto mw = *it;
                    // Create a new handler that calls the middleware with current_handler as 'next'
                    // We need to capture mw and current_handler
                    current_handler = [mw, current_handler](const Request& r) -> Response {
                        // Middleware takes (Request&, RouteHandler)
                        // But here we have const Request&.
                        // We must cast away constness because middleware signature expects Request&.
                        // This is safe because `route` takes `Request&` and passes it down.
                        // The initial call to `current_handler(req)` inside `route` passes the non-const `req` as const reference.
                        // We need to be careful.
                        
                        // Actually, let's fix the middleware signature or the call.
                        // Middleware: Response(Request&, RouteHandler)
                        // RouteHandler: Response(const Request&)
                        
                        // If we are inside the chain, we receive `const Request& r`.
                        // We want to call `mw((Request&)r, current_handler)`.
                        // We need to const_cast. This is ugly but necessary if we want middleware to modify request
                        // while maintaining `const Request&` for the final handler.
                        // Alternatively, RouteHandler should take `Request&`.
                        // But the user specified `RouteHandler = std::function<Response(const Request&)>;`
                        
                        // If the user wants middleware to modify request, then RouteHandler should probably take Request& too.
                        // But maybe the final handler shouldn't modify request.
                        
                        // Let's assume we can cast for middleware purposes, since `route` owns the mutable request.
                        Request& mutable_req = const_cast<Request&>(r);
                        return mw(mutable_req, current_handler);
                    };
                }
                
                return current_handler(req);
            }
        }
    }
    return std::nullopt;
}

bool Router::match_path(const Route& route, const std::string& path, 
                       std::unordered_map<std::string, std::string>& params) {
    if (!route.is_regex) {
        return path == route.path;
    }
    
    std::smatch match;
    if (std::regex_match(path, match, route.pattern)) {
        for (size_t i = 0; i < route.param_names.size(); ++i) {
            if (i + 1 < match.size()) {
                params[route.param_names[i]] = match[i + 1].str();
            }
        }
        return true;
    }
    return false;
}

std::string Router::regex_from_path(const std::string& path, std::vector<std::string>& param_names) {
    std::string regex = "^";
    std::string segment;
    std::stringstream ss(path);
    
    // Handle root path specially
    if (path == "/") return "^/$";
    
    // If path doesn't start with /, treat it as relative? No, assume absolute.
    
    // Split by /
    // This is a simple implementation.
    // Iterating char by char is better.
    
    for (size_t i = 0; i < path.length(); ++i) {
        if (path[i] == ':') {
            // Start of a parameter
            size_t j = i + 1;
            while (j < path.length() && (isalnum(path[j]) || path[j] == '_')) {
                j++;
            }
            std::string param_name = path.substr(i + 1, j - i - 1);
            param_names.push_back(param_name);
            regex += "([^/]+)";
            i = j - 1;
        } else if (path[i] == '*') {
            // Wildcard
            regex += ".*";
        } else {
            // Escape special regex chars if needed
            // For now assume simple paths
            regex += path[i];
        }
    }
    
    regex += "$";
    return regex;
}

} // namespace server
} // namespace http
