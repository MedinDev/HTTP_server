#include "server/router.hpp"

namespace http {
namespace server {

void Router::add_route(const std::string& method, const std::string& path, Handler handler) {
    routes_[method][path] = handler;
}

void Router::route(const Request& req, Response& res) {
    if (routes_.count(req.method) && routes_[req.method].count(req.path)) {
        routes_[req.method][req.path](req, res);
    } else {
        res.status_code = 404;
        res.status_message = "Not Found";
        res.body = "404 Not Found";
    }
}

}
}
