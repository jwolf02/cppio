#ifndef __IODEVICE_HPP
#define __IODEVICE_HPP

#include <cstddef>
#include <cstdint>

class IODevice {
public:

    IODevice() = default;

    IODevice(IODevice &&dev) noexcept;

    ~IODevice();

    IODevice& operator=(IODevice &&dev) noexcept;

    size_t read(void *buf, size_t len) const;

    size_t write(const void *buf, size_t len) const;

    void close();

    bool is_open() const;

    bool operator!() const;

    bool readable(uint32_t timeout=0) const;

    bool writable(uint32_t timeout=0) const;

    bool available(uint32_t timeout=0) const;

    void swap(IODevice &dev);

protected:

    int _fd = -1;

};

#endif // __IODEVICE_HPP
