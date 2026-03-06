#pragma once
#include <unistd.h>

namespace http {
namespace server {

class Connection {
public:
    Connection(int socket) : socket_(socket) {}
    ~Connection() { if(socket_ != -1) close(socket_); }
    
    int get_socket() const { return socket_; }
    
private:
    int socket_;
};

}
}
