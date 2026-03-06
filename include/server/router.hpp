#pragma once
#include <string>
#include <functional>
#include <map>
#include "server/request.hpp"
#include "server/response.hpp"

namespace http {
namespace server {

using Handler = std::function<void(const Request&, Response&)>;

class Router {
public:
    void add_route(const std::string& method, const std::string& path, Handler handler);
    void route(const Request& req, Response& res);

private:
    std::map<std::string, std::map<std::string, Handler>> routes_;
};

}
}
