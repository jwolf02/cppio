#ifndef __SERIAL_HPP
#define __SERIAL_HPP

#include <string>
#include <IODevice.hpp>

class Serial : public IODevice {
public:

    Serial() = default;

    explicit Serial(const std::string &devname, uint32_t baudrate=9600);

    void open(const std::string &devname, uint32_t baudrate=9600);

};

#endif // __SERIAL_HPP
