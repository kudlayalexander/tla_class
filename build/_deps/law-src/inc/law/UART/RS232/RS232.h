#pragma once

#include <array>

#include "law/Utils.h"
#include "law/UART/Base/UartBase.h"
#include "law/RetCode/RetCode.h"
#include "law/FileDescriptor/FileDescriptor.h"

namespace law::uart::rs232 {
    template <std::size_t bufSize>
    class StaticRS232 final : public law::uart::detail::UartBase {
    private:
        using Base = law::uart::detail::UartBase;
        using ReadBuffer = std::array<char, bufSize + 1>;

        using Base::read;

    private:
        ReadBuffer readBuffer;

    public:
        using Base::Base;

        [[nodiscard]] BlobViewRet read() noexcept {
            BOOST_LEAF_AUTO(availableBytes, utils::getAvailableBytesForRead(getNativeHandler()));
            return (availableBytes == 0) ? read(getDefaultReadCount()) : read((availableBytes > bufSize ? bufSize : availableBytes));
        }

        [[nodiscard]] BlobViewRet read(std::size_t size) noexcept {
            if (size > bufSize) {
                return boost::leaf::new_error(RetCode::NO_AVAILABLE_SPACE);
            }

            BOOST_LEAF_AUTO(recBytes, Base::read(&readBuffer[0], size));

            readBuffer[recBytes] = '\0';
            return std::string_view{readBuffer.data(), recBytes};
        }
    };

    class RS232 final : public law::uart::detail::UartBase {
    private:
        using Base = law::uart::detail::UartBase;

        using Base::read;

    public:
        using Base::Base;

        [[nodiscard]] BlobRet read() const noexcept;

        [[nodiscard]] BlobRet read(std::size_t size) const noexcept;
    };

    using ExtBufferRS232 =  law::uart::detail::UartBase;
}