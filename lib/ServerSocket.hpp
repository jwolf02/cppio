#ifndef __SERVERSOCKET_HPP
#define __SERVERSOCKET_HPP

#include <Socket.hpp>
#include <netinet/in.h>

class ServerSocket {
public:

    ServerSocket() = default;

    explicit ServerSocket(int port, int backlog=1);

    ~ServerSocket();

    void listen(int port, int backlog=1);

    bool is_open() const;

    bool operator!() const;

    Socket wait_for_connection();

    bool connection_available() const;

    int port() const;

    void close();

private:

    int _fd = -1;

    struct sockaddr_in _addr = { 0 };

};

#endif // __SERVERSOCKET_HPP
