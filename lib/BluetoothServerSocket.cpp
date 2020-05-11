#include <BluetoothServerSocket.hpp>
#include <stdexcept>
#include <cstring>
#include <bluetooth/bluetooth.h>

BluetoothServerSocket::BluetoothServerSocket(int channel, int backlog) {
    listen(channel, backlog);
}

BluetoothServerSocket::~BluetoothServerSocket() {
    close();
}

void BluetoothServerSocket::listen(int channel, int backlog) {
    // allocate socket
    if ((_fd = ::socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)) < 0) {
        throw std::runtime_error(strerror(errno));
    }

    // bind socket to port 1 of the first available
    // local bluetooth adapter
    _addr.rc_family = AF_BLUETOOTH;
    str2ba("XX:XX:XX:XX:XX:XX", (bdaddr_t *) &_addr.rc_bdaddr);
    _addr.rc_channel = (uint8_t) 1;
    _addr.rc_channel = (uint8_t) 1;
    if (bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr))) {
        throw std::runtime_error(strerror(errno));
    }

    // put socket into listening mode
    if (::listen(_fd, backlog)) {
        throw std::runtime_error(strerror(errno));
    }
}

bool BluetoothServerSocket::is_open() const {
    return _fd >= 0;
}

bool BluetoothServerSocket::operator!() const {
    return is_open();
}

BluetoothSocket BluetoothServerSocket::wait_for_connection() const {
    BluetoothSocket socket;
    socklen_t len = sizeof(socket._addr);
    socket._fd = accept(_fd, (struct sockaddr *) &socket._addr, &len);
    if (socket._fd <= 0) {
        throw std::runtime_error(strerror(errno));
    }
    return socket;
}

bool BluetoothServerSocket::connection_available() const {
    fd_set set;
    FD_ZERO(&set);
    FD_SET(_fd, &set);

    struct timeval tv = {(long int) 0, (long int) 0};

    int ret = select(FD_SETSIZE, &set, nullptr, nullptr, &tv);
    if (ret == 0) {
        // timeout
        return false;
    } else if (ret < 0) {
        throw std::runtime_error(strerror(errno));
    }

    return true;
}

int BluetoothServerSocket::channel() const {
    return _addr.rc_channel;
}

void BluetoothServerSocket::close() {
    if (_fd)
}