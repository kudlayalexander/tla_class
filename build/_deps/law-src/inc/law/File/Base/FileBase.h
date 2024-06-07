#pragma once

#include "law/Types.h"

#include "law/File/FileTypes.h"
#include "law/File/OpenOption.h"
#include "law/File/Permission.h"

#include "law/FileDescriptor/FileDescriptor.h"

namespace law::file::detail {
    class FileBase : public fd::FileDescriptor {
    private:
        using Base = fd::FileDescriptor;

    public:
        FileBase() noexcept = default;

        explicit FileBase(std::string_view filename, OpenOption options = ReadWrite(), Permission permissions = OwnerReadWriteExec());

        /**
         * @return file permissions
         */
        [[nodiscard]] FilePermissions getPermissions() const noexcept;

        /**
         * @return true - On success
         * @return EC::X_EACCES
         * @return EC::X_EBADF
         * @return EC::X_EBUSY
         * @return EC::X_EDQUOT
         * @return EC::X_EEXIST
         * @return EC::X_EFAULT
         * @return EC::X_EFBIG
         * @return EC::X_EINTR
         * @return EC::X_EINVAL
         * @return EC::X_EISDIR
         * @return EC::X_ELOOP
         * @return EC::X_EMFILE
         * @return EC::X_ENAMETOOLONG
         * @return EC::X_ENFILE
         * @return EC::X_ENODEV
         * @return EC::X_ENOENT
         * @return EC::X_ENOMEM
         * @return EC::X_ENOSPC
         * @return EC::X_ENOTDIR
         * @return EC::X_ENXIO
         * @return EC::X_EOPNOTSUPP
         * @return EC::X_EOVERFLOW
         * @return EC::X_EPERM
         * @return EC::X_EROFS
         * @return EC::X_ETXTBSY
         * @return EC::X_EWOULDBLOCK
         * */
        Status open(std::string_view filename, const OpenOption &options, const Permission &perms = Permission(0)) noexcept;

        /**
         * @return true - On success
         * @return EBADF
         * @return EINTR
         * @return EIO
         * @return ENOSPC
         * @return EDQUOT
         * */
         using Base::close;

        /**
         * @return true - On success
         * @return EAGAIN
         * @return EBADF
         * @return EDESTADDRREQ
         * @return EDQUOT
         * @return EFAULT
         * @return EFBIG
         * @return EINTR
         * @return EINVAL
         * @return EIO
         * @return ENOSPC
         * @return EPERM
         * @return EPIPE
         * */
        Status write(std::string_view buffer) noexcept;

        /**
         * @return true - On success
         * @return EAGAIN
         * @return EBADF
         * @return EDESTADDRREQ
         * @return EDQUOT
         * @return EFAULT
         * @return EFBIG
         * @return EINTR
         * @return EINVAL
         * @return EIO
         * @return ENOSPC
         * @return EPERM
         * @return EPIPE
         * */
        Status write(const void *buffer, std::size_t bufferSize) noexcept;

        /**
         * @return true - On success
         * @return EC::X_EBADF
         * @return EC::X_EROFS
         * @return EC::X_EINVAL
         * @return EC::X_EIO
         * */
        [[nodiscard]] Status flush() const noexcept;

        /**
         * @return true - On success
         * @return EC::X_EBADF
         * @return EC::X_EIO
         * @return EC::X_EOVERFLOW
         * */
        [[nodiscard]] FileSize getFileSize() const noexcept;

        /**
         * @return true - On success
         * @return EC::X_EINTR
         * @return EC::X_EINVAL
         * @return EC::X_EFBIG
         * @return EC::X_EIO
         * @return EC::X_EBADF
         * */
        [[nodiscard]] Status resize(std::size_t newSize) const noexcept;

        /**
         * @return true - On success
         * @return EC::X_ESPIPE
         * @return EC::X_EINVAL
         * @return EC::X_EOVERFLOW
         * */
        Status setPosition(std::size_t pos) noexcept;

        /**
         * @return all file content as string - On success
         * @return EC::X_EAGAIN
         * @return EC::X_EWOULDBLOCK
         * @return EC::X_EBADF
         * @return EC::X_EFAULT
         * @return EC::X_EINTR
         * @return EC::X_EINVAL
         * @return EC::X_EIO
         * @return EC::X_EISDIR
         * @return EC::X_EOVERFLOW
         * */
        ReadCountRet read(void *extBuffer, std::size_t size) const noexcept;
    };
}