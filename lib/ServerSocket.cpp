#include <ServerSocket.hpp>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <stdexcept>

#define SYSTEM_ERROR    throw std::runtime_error(strerror(errno))

ServerSocket::ServerSocket(int port, int backlog) {
    listen(port, backlog);
}

ServerSocket::~ServerSocket() {
    close();
}

void ServerSocket::listen(int port, int backlog) {
    if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        SYSTEM_ERROR;
    }

    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = INADDR_ANY;
    _addr.sin_port = htons(port);

    if (bind(_fd, (struct sockaddr *) &_addr, sizeof(_addr)) < 0) {
        _fd = -1;
        SYSTEM_ERROR;
    }

    if (::listen(_fd, backlog) < 0) {
        _fd = -1;
        SYSTEM_ERROR;
    }
}

bool ServerSocket::is_open() const {
    return _fd >= 0;
}

bool ServerSocket::operator!() const {
    return is_open();
}

Socket ServerSocket::wait_for_connection() {
    Socket socket;
    socklen_t len = sizeof(socket._addr);
    socket._fd = accept(_fd, (struct sockaddr *) &socket._addr, &len);
    if (socket._fd <= 0) {
        SYSTEM_ERROR;
    }
    return socket;
}

bool ServerSocket::connection_available() const {
    fd_set set;
    FD_ZERO(&set);
    FD_SET(_fd, &set);

    struct timeval tv = {(long int) 0, (long int) 0};

    int ret = select(FD_SETSIZE, &set, nullptr, nullptr, &tv);
    if (ret == 0) {
        // timeout
        return false;
    } else if (ret < 0) {
        SYSTEM_ERROR;
    }

    return true;
}

int ServerSocket::port() const {
    return _addr.sin_port;
}

void ServerSocket::close() {
    if (is_open()) {
        ::close(_fd);
        _fd = -1;
    }
    _addr = { 0 };
}
