#include "handlers/static_file_handler.hpp"
#include <fstream>
#include <sstream>

namespace http {
namespace handlers {

void StaticFileHandler::operator()(const server::Request& req, server::Response& res) {
    std::string path = root_dir_ + req.path;
    // Basic security check to prevent directory traversal
    if (path.find("..") != std::string::npos) {
        res.status_code = 403;
        res.status_message = "Forbidden";
        res.body = "Access Denied";
        return;
    }

    if (path.back() == '/') path += "index.html";

    std::ifstream file(path);
    if (file) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        res.body = buffer.str();
        res.status_code = 200;
        res.status_message = "OK";
    } else {
        res.status_code = 404;
        res.status_message = "Not Found";
        res.body = "File not found";
    }
}

}
}
