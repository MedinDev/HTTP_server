#pragma once
#include <string>
#include <map>

namespace http {
namespace server {

class Response {
public:
    int status_code = 200;
    std::string status_message = "OK";
    std::map<std::string, std::string> headers;
    std::string body;

    std::string to_string() const;
};

}
}
