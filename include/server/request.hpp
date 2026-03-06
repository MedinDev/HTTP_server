#pragma once
#include <string>
#include <map>

namespace http {
namespace server {

class Request {
public:
    std::string method;
    std::string path;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;

    static Request parse(const std::string& raw_data);
};

}
}
