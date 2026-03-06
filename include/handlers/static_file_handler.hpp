#pragma once
#include "server/request.hpp"
#include "server/response.hpp"
#include <string>

namespace http {
namespace handlers {

class StaticFileHandler {
public:
    StaticFileHandler(const std::string& root_dir) : root_dir_(root_dir) {}
    void operator()(const server::Request& req, server::Response& res);
private:
    std::string root_dir_;
};

}
}
