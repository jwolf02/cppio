#include <IODevice.hpp>
#include <unistd.h>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <string>

IODevice::IODevice(IODevice &&dev) noexcept {
    swap(dev);
}

IODevice::~IODevice() {
    close();
}

IODevice& IODevice::operator=(IODevice &&dev) noexcept {
    swap(dev);
    return *this;
}

size_t IODevice::read(void *buf, size_t len) const {
    ssize_t ret = ::read(_fd, buf, len);
    if (ret < 0) {
        throw std::runtime_error(strerror(errno));
    }
    return (size_t) ret;
}

size_t IODevice::write(const void *buf, size_t len) const {
    ssize_t ret = ::write(_fd, buf, len);
    if (ret < (ssize_t) len && ret != 0) {
        throw std::runtime_error(strerror(errno));
    }
    return (size_t) ret;
}

void IODevice::close() {
    if (is_open()) {
        ::close(_fd);
    }
    _fd = -1;
}

bool IODevice::is_open() const {
    return _fd >= 0;
}

bool IODevice::operator!() const {
    return is_open();
}

bool IODevice::readable(uint32_t timeout) const {
    fd_set set;
    FD_ZERO(&set);
    FD_SET(_fd, &set);

    struct timeval tv = {(long int) timeout, (long int) 0};

    int ret = select(FD_SETSIZE, &set, nullptr, nullptr, &tv);
    if (ret == 0) {
        // timeout
        return false;
    } else if (ret < 0) {
        throw std::runtime_error(strerror(errno));
    }

    return true;
}

bool IODevice::writable(uint32_t timeout) const {
    fd_set set;
    FD_ZERO(&set);
    FD_SET(_fd, &set);

    struct timeval tv = {(long int) timeout, (long int) 0};

    int ret = select(FD_SETSIZE, nullptr, &set, nullptr, &tv);
    if (ret == 0) {
        // timeout
        return false;
    } else if (ret < 0) {
        throw std::runtime_error(strerror(errno));
    }

    return true;
}

bool IODevice::available(uint32_t timeout) const {
    fd_set set;
    FD_ZERO(&set);
    FD_SET(_fd, &set);

    struct timeval tv = {(long int) timeout, (long int) 0};

    int ret = select(FD_SETSIZE, &set, &set, nullptr, &tv);
    if (ret == 0) {
        // timeout
        return false;
    } else if (ret < 0) {
        throw std::runtime_error(strerror(errno));
    }

    return true;
}

void IODevice::swap(IODevice &dev) {
    std::swap(_fd, dev._fd);
}
