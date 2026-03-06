#include "server/response.hpp"
#include <sstream>

namespace http {
namespace server {

std::string Response::to_string() const {
    std::stringstream ss;
    ss << "HTTP/1.1 " << status_code << " " << status_message << "\r\n";
    
    // Add Content-Length if not already present
    if (headers.find("Content-Length") == headers.end()) {
        ss << "Content-Length: " << body.length() << "\r\n";
    }

    for (const auto& [key, value] : headers) {
        ss << key << ": " << value << "\r\n";
    }
    ss << "\r\n";
    ss << body;
    return ss.str();
}

}
}
