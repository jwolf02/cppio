#include <I2CDev.hpp>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/i2c-dev.h>
#include <stdexcept>
#include <cstring>

const std::string I2CDev::DEFAULT_NAME = "/dev/i2c-1";

std::vector<int> I2CDev::list_devices() {
    std::vector<int> addresses;
    for (int i = 1; i < 128; ++i) {
        I2CDev dev;
        try {
            dev.open(DEFAULT_NAME, i);
        } catch (std::exception &ex) {
            continue;
        }
        // successfull
        addresses.push_back(i);
    }
    return addresses;
}

I2CDev::I2CDev(const std::string &devname, int address) {
    open(devname, address);
}

void I2CDev::open(const std::string &devname, int address) {
    _fd = ::open(devname.c_str(), O_RDWR);
    if (_fd >= 0) {
        _addr = address;
        if (ioctl(_fd, I2C_SLAVE, address) < 0) {
            throw std::runtime_error(strerror(errno));
        }
    }

    // check if device is available
    char null = 0x00;
    write(&null, 1);
}

int I2CDev::address() const {
    return _addr;
}
