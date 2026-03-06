#pragma once
#include "server/request.hpp"
#include "server/response.hpp"

namespace http {
namespace handlers {

// Base class or just utility functions
void default_handler(const server::Request& req, server::Response& res);

}
}
