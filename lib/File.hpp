#ifndef __FILE_HPP
#define __FILE_HPP

#include <IODevice.hpp>
#include <string>
#include <fcntl.h>
#include <vector>

class File : public IODevice {
public:

    enum {
        READONLY = O_RDONLY,
        WRITEONLY = O_WRONLY,
        READWRITE = O_RDWR,
        APPEND = O_APPEND,
        SYNC = O_SYNC,
        TRUNCATE = O_TRUNC,
        NONBLOCK = O_NONBLOCK
    };

    static bool exists(const std::string &fname);

    static bool remove(const std::string &fname);

    File() = default;

    explicit File(const std::string &fname, int flags=READWRITE);

    void open(const std::string &fname, int flags=READWRITE);

    size_t size() const;

    size_t seek(size_t pos, int offset=SEEK_SET) const;

    bool is_file() const;

    bool is_directory() const;

    std::vector<std::string> list_directory() const;

    int owner_id() const;

    int group_id() const;

};

#endif // __FILE_HPP
