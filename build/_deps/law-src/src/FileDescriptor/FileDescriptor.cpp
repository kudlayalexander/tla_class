#include <unistd.h>

#include "law/RetCode/RetCode.h"
#include "law/FileDescriptor/FileDescriptor.h"

namespace law::fd {
    FileDescriptor::FileDescriptor(int fd) : fd(fd) {}

    FileDescriptor::FileDescriptor(FileDescriptor &&fileDescriptor) noexcept: fd(fileDescriptor.fd) {
        fileDescriptor.fd = -1;
    }

    FileDescriptor &FileDescriptor::operator=(FileDescriptor &&rhs) noexcept {
        if (this != &rhs) {
            fd = rhs.fd;
            rhs.fd = -1;
        }
        return *this;
    }

    FileDescriptor::~FileDescriptor() noexcept {
        close();
    }

    [[nodiscard]] int FileDescriptor::getNativeHandler() const noexcept {
        return fd;
    }

    FileDescriptor::operator bool() const noexcept {
        return fd != -1;
    }

    void FileDescriptor::setNativeHandler(int *handler) noexcept {
        fd = *handler;
        *handler = -1;
    }

    Status FileDescriptor::close() noexcept {
        if (fd == -1) {
            return {};
        }

        if (::close(fd) == -1) {
            return fromErrno();
        }

        fd = -1;
        return {};
    }
}