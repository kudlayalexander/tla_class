#include <fcntl.h>
#include <sys/stat.h>

#include "law/RetCode/RetCode.h"
#include "law/File/Base/FileBase.h"

namespace law::file::detail {
    FileBase::FileBase(std::string_view filename, OpenOption options, Permission permissions) :
            Base(::open(filename.data(), options.getAsIntegral(), permissions.getAsIntegral())) {
        if (!operator bool()) {
            throw std::system_error(errno, std::system_category());
        }
    }

    [[nodiscard]] FilePermissions FileBase::getPermissions() const noexcept {
        struct stat st{};
        if (fstat(this->getNativeHandler(), &st)) {
            return fromErrno();
        }
        return Permission((st.st_mode & 0777));
    }

    Status FileBase::open(std::string_view filename, const OpenOption &options, const Permission &perms) noexcept {
        int fd = ::open(filename.data(), options.getAsIntegral(), perms.getAsIntegral());
        if (fd == -1) {
            return fromErrno();
        }

        setNativeHandler(&fd);
        return {};
    }

    Status FileBase::write(std::string_view buffer) noexcept {
        return write(buffer.data(), buffer.size());
    }

    Status FileBase::write(const void *buffer, std::size_t bufferSize) noexcept {
        return (::write(getNativeHandler(), buffer, bufferSize) == -1) ? fromErrno() : Status();
    }

    [[nodiscard]] Status FileBase::flush() const noexcept {
        return ((fsync(this->getNativeHandler()) == -1) ? fromErrno() : Status());
    }

    [[nodiscard]] FileSize FileBase::getFileSize() const noexcept {
        struct stat st{};
        if (fstat(this->getNativeHandler(), &st) == -1) {
            return fromErrno();
        }
        return st.st_size;
    }

    [[nodiscard]] Status FileBase::resize(std::size_t newSize) const noexcept {
        return ((ftruncate(this->getNativeHandler(), long(newSize)) == -1) ? fromErrno() : Status());
    }

    Status FileBase::setPosition(std::size_t pos) noexcept {
        return lseek(this->getNativeHandler(), int(pos), SEEK_SET) == -1 ? fromErrno() : Status();
    }

    ReadCountRet FileBase::read(void *extBuffer, std::size_t size) const noexcept {
        const auto recBytes = ::read(getNativeHandler(), extBuffer, size);
        if (recBytes == -1) {
            return fromErrno();
        }

        return recBytes;
    }
}