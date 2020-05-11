#include <BluetoothSocket.hpp>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include <stdexcept>

std::vector<std::pair<std::string, std::string>> BluetoothSocket::discover_devices() {
    char addr[20] = { 0 };
    char name[248] = { 0 };

    int dev_id = hci_get_route(nullptr);
    if (dev_id < 0) {
        throw std::runtime_error("no bluetooth adapter found");
    }

    int sock = hci_open_dev(dev_id);
    if (sock < 0) {
        throw std::runtime_error(strerror(errno));
    }

    int len  = 8;
    int max_rsp = 255;
    int flags = IREQ_CACHE_FLUSH;
    auto *ii = new inquiry_info[max_rsp];

    int num_rsp = hci_inquiry(dev_id, len, max_rsp, nullptr, &ii, flags);
    if(num_rsp < 0) {
        throw std::runtime_error(strerror(errno));
    }

    std::vector<std::pair<std::string, std::string>> devices;
    for (int i = 0; i < num_rsp; i++) {
        ba2str(&(ii+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), name, 0) < 0) {
            devices.emplace_back("[unknown]", addr);
        } else {
            devices.emplace_back(name, addr);
        }
    }

    delete[] ii;
    ::close(sock);

    return devices;
}

BluetoothSocket BluetoothSocket::connect(const std::string &addr, int channel) {
    BluetoothSocket socket;
    if ((socket._fd = ::socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)) < 0) {
        throw std::runtime_error(strerror(errno));
    }

    // set the connection parameters (who to connect to)
    socket._addr.rc_family = AF_BLUETOOTH;
    socket._addr.rc_channel = (uint8_t) 1;
    str2ba(addr.c_str(), (bdaddr_t*) &socket._addr.rc_bdaddr);

    // connect to server
    if (::connect(socket._fd, (struct sockaddr *)&addr, sizeof(addr))) {
        throw std::runtime_error(strerror(errno));
    }
    return socket;
}

std::string BluetoothSocket::address() const {
    char buf[20] = { 0 };
    ba2str((const bdaddr_t *) _addr.rc_bdaddr, buf);
    return std::string(buf);
}

int BluetoothSocket::channel() const {
    return _addr.rc_channel;
}
