#include <Socket.hpp>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <stdexcept>

Socket Socket::connect(const std::string &address, int port) {
    Socket socket;
    socket._fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket._fd < 0) {
        throw std::runtime_error(strerror(errno));
    }

    socket._addr.sin_addr.s_addr = inet_addr(address.c_str());
    socket._addr.sin_family = AF_INET;
    socket._addr.sin_port = htons(port);

    //Connect to remote server
    if (::connect(socket._fd ,(struct sockaddr *) &socket._addr, sizeof(socket._addr)) < 0) {
        socket._fd = -1;
        throw std::runtime_error(strerror(errno));
    }

    return socket;
}

std::string Socket::address() const {
    char str[INET_ADDRSTRLEN];
    inet_ntop( AF_INET, &_addr.sin_addr, str, INET_ADDRSTRLEN);
    return std::string(str);
}

int Socket::port() const {
    return _addr.sin_port;
}
