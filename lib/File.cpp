#include <File.hpp>
#include <stdexcept>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <dirent.h>

bool File::exists(const std::string &fname) {
    return !::access(fname.c_str(), F_OK);
}

bool File::remove(const std::string &fname) {
    return !::remove(fname.c_str());
}

File::File(const std::string &fname, int flags) {
    open(fname, flags);
}

void File::open(const std::string &fname, int flags) {
    _fd = ::open(fname.c_str(), flags);
    if (_fd < 0) {
        throw std::runtime_error(strerror(errno));
    }
}

size_t File::size() const {
    struct stat st = { 0 };
    if (fstat(_fd, &st)) {
        throw std::runtime_error(strerror(errno));
    }
    return (size_t) st.st_size;
}

size_t File::seek(size_t pos, int offset) const {
    return (size_t) lseek64(_fd, offset, pos);
}

bool File::is_file() const {
    return !is_directory();
}

bool File::is_directory() const {
    struct stat st = { 0 };
    if (fstat(_fd, &st)) {
        throw std::runtime_error(strerror(errno));
    }
    return S_ISDIR(st.st_mode);
}

std::vector<std::string> File::list_directory() const {
    if (!is_directory()) {
        return std::vector<std::string>();
    }
    DIR *d = fdopendir(_fd);
    if (d == nullptr) {
        throw std::runtime_error(strerror(errno));
    }
    std::vector<std::string> fnames;
    struct dirent *dir;
    while ((dir = readdir(d)) != nullptr) {
        fnames.emplace_back(dir->d_name);
    }
    closedir(d);
    return fnames;
}

int File::owner_id() const {
    struct stat st = { 0 };
    if (fstat(_fd, &st)) {
        throw std::runtime_error(strerror(errno));
    }
    return (int) st.st_uid;
}

int File::group_id() const {
    struct stat st = { 0 };
    if (fstat(_fd, &st)) {
        throw std::runtime_error(strerror(errno));
    }
    return (int) st.st_gid;
}
