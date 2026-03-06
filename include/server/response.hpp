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

    static Response json(const std::string& json_body, int status = 200) {
        Response res;
        res.status_code = status;
        res.status_message = (status == 200) ? "OK" : "Status";
        res.headers["Content-Type"] = "application/json";
        res.body = json_body;
        return res;
    }
};

}
}
