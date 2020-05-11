#ifndef __BLUETOOTHSOCKET_HPP
#define __BLUETOOTHSOCKET_HPP

#include <string>
#include <IODevice.hpp>
#include <vector>
#include <string>
#include <bluetooth/rfcomm.h>

class BluetoothServerSocket;

class BluetoothSocket : public IODevice {
public:

    friend BluetoothServerSocket;

    static std::vector<std::pair<std::string, std::string>> discover_devices();

    static BluetoothSocket connect(const std::string &addr, int channel);

    BluetoothSocket() = default;

    std::string address() const;

    int channel() const;

private:

    struct sockaddr_rc _addr = { 0 };

};

#endif // __BLUETOOTHSOCKET_HPP
