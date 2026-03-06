#include "handlers/route_handler.hpp"

namespace http {
namespace handlers {

void default_handler(const server::Request& req, server::Response& res) {
    res.body = "Hello from default handler!";
}

}
}
