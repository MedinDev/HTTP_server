#pragma once
#include <string>
#include <map>
#include <unordered_map>

namespace http {
namespace server {

class Request {
public:
    std::string method;
    std::string path;
    std::string version;
    std::map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> path_params;
    std::unordered_map<std::string, std::string> query_params;
    std::string body;

    static Request parse(const std::string& raw_data);
};

} // namespace server
} // namespace http
