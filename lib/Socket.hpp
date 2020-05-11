#ifndef __SOCKET_HPP
#define __SOCKET_HPP

#include <IODevice.hpp>
#include <cstddef>
#include <cstdint>
#include <string>
#include <netinet/in.h>

class ServerSocket;

class Socket : public IODevice {
public:

    friend ServerSocket;

    static Socket connect(const std::string &address, int port);

    Socket() = default;

    std::string  address() const;

    int port() const;

private:

    struct sockaddr_in _addr = { 0 };

};

#endif // __SOCKET_HPP
