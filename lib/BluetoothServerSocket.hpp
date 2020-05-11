#ifndef __BLUETOOTHSERVERSOCKET_HPP
#define __BLUETOOTHSERVERSOCKET_HPP

#include <string>
#include <IODevice.hpp>
#include <BluetoothSocket.hpp>

class BluetoothServerSocket {
public:

    BluetoothServerSocket() = default;

    BluetoothServerSocket(int channel, int backlog=1);

    ~BluetoothServerSocket();

    void listen(int channel, int backlog=1);

    bool is_open() const;

    bool operator!() const;

    BluetoothSocket wait_for_connection() const;

    bool connection_available() const;

    int channel() const;

    void close();

private:

    int _fd = -1;

    struct sockaddr_rc _addr = { 0 };

};

#endif // __BLUETOOTHSERVERSOCKET_HPP
