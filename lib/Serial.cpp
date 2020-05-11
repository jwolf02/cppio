#include <fcntl.h>
#include <termios.h>
#include <cstring>
#include <Serial.hpp>
#include <cerrno>
#include <stdexcept>

Serial::Serial(const std::string &devname, uint32_t baudrate) {
    open(devname, baudrate);
}

void Serial::open(const std::string &devname, uint32_t baudrate) {
    if (( _fd = ::open(devname.c_str(), O_RDWR | O_NOCTTY | O_NDELAY)) < 0) {
        throw std::runtime_error(strerror(errno));
    }

    static const int baudrates[] = {
            0, 50, 75, 110, 134, 150, 200, 300, 600, 1200, 1800, 2400, 4800, 9600, 19200, 384000, 576000, 115200
    };

    int rate = -1;
    for (int i = 0; i < 18; ++i) {
        if (baudrate == baudrates[i]) {
            rate = i;
            break;
        }
    }

    if (rate < 0) {
        close();
        throw std::runtime_error("invalid baudrate");
    }

    // configure file descriptor
    struct termios options = { 0 };
    if (tcgetattr(_fd, &options)) {
        throw std::runtime_error(strerror(errno));
    }
    options.c_cflag = rate | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    if (tcflush(_fd, TCIFLUSH)) {
        throw std::runtime_error(strerror(errno));
    }
    if (tcsetattr(_fd, TCSANOW, &options)) {
        throw std::runtime_error(strerror(errno));
    }
}
