#pragma once

#include <atomic>
#include <chrono>

namespace http {
namespace server {

struct Metrics {
    std::atomic<uint64_t> total_requests{0};
    std::atomic<uint64_t> active_connections{0};
    std::atomic<uint64_t> bytes_sent{0};
    std::chrono::milliseconds uptime{0};
};

} // namespace server
} // namespace http
