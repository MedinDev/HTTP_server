#include "server/request.hpp"
#include <sstream>
#include <vector>

namespace http {
namespace server {

Request Request::parse(const std::string& raw_data) {
    Request req;
    std::istringstream stream(raw_data);
    std::string line;

    // Parse request line
    if (std::getline(stream, line)) {
        std::istringstream line_stream(line);
        line_stream >> req.method >> req.path >> req.version;
    }

    // Parse headers
    while (std::getline(stream, line) && line != "\r") {
        auto colon = line.find(':');
        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string value = line.substr(colon + 1);
            // Trim whitespace
            size_t first = value.find_first_not_of(" \t\r");
            if (first != std::string::npos) {
                value = value.substr(first);
            }
            size_t last = value.find_last_not_of(" \t\r");
            if (last != std::string::npos) {
                value = value.substr(0, last + 1);
            }
            req.headers[key] = value;
        }
    }

    // Parse body (simplified)
    if (stream) {
        std::string body_content((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
        req.body = body_content;
    }

    return req;
}

}
}
