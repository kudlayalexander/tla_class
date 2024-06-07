#pragma once

#include <string>
#include <etl/string.h>

#include "law/Types.h"
#include "law/Utils.h"
#include "Base/FileBase.h"
#include "law/RetCode/RetCode.h"

namespace law::file {
    template <std::size_t bufSize>
    class StaticFile final : public detail::FileBase {
    private:
        using Base = detail::FileBase;

        using ReadBuffer = std::array<char, bufSize + 1>;

        using Base::read;

    private:
        ReadBuffer readBuffer;

    public:
        StaticFile() noexcept = default;

        explicit
        StaticFile(std::string_view fn, OpenOption openFlag = ReadWrite(), Permission permissions = OwnerReadWriteExec()) :
                Base(fn, openFlag, permissions) { }

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
        [[nodiscard]] BlobViewRet read() noexcept {
            BOOST_LEAF_AUTO(fileSize, this->getFileSize());
            return read(fileSize);
        }

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
        [[nodiscard]] BlobViewRet read(std::size_t size) noexcept {
            if (size > bufSize) {
                return boost::leaf::new_error(RetCode::NO_AVAILABLE_SPACE);
            }

            BOOST_LEAF_AUTO(recBytes, Base::read(readBuffer.data(), size));

            readBuffer[recBytes] = '\0';
            return std::string_view{readBuffer.data(), std::string_view::size_type(recBytes)};
        }
    };

    class File final : public detail::FileBase {
    private:
        using Base = detail::FileBase;

    public:
        File() noexcept = default;

        template <StringLike T>
        explicit
        File(T &&filename, OpenOption openFlag = ReadWrite(), Permission permissions = OwnerReadWriteExec()) :
                Base(filename, openFlag, permissions) {}

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
        [[nodiscard]] BlobRet read() const noexcept;

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
        [[nodiscard]] BlobRet read(std::size_t size) const noexcept;
    };

    using ExtBufferFile = detail::FileBase;
}