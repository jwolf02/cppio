#ifndef __I2C_HPP
#define __I2C_HPP

#include <IODevice.hpp>
#include <string>
#include <cstddef>
#include <vector>

class I2CDev : public IODevice {
public:

    static const std::string DEFAULT_NAME;

    static std::vector<int> list_devices();

    I2CDev() = default;

    I2CDev(const std::string &devname, int address);

    void open(const std::string &devname, int address);

    int address() const;

private:

    int _addr = 0;

};

#endif // __I2C_HPP
