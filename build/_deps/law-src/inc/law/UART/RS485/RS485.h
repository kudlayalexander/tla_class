#pragma once

#include <array>

#include "Base/RS485Base.h"
#include "law/FileDescriptor/FileDescriptor.h"
#include "law/RetCode/RetCode.h"
#include "law/UART/RS485/RS485Configuration.h"
#include "law/UART/UartConfiguration.h"
#include "law/Utils.h"

namespace law::uart::rs485 {
    template <std::size_t bufSize>
    class StaticRS485 final : public detail::RS485Base {
    private:
        using Base = detail::RS485Base;
        using ReadBuffer = std::array<char, bufSize + 1>;

        using Base::read;

    private:
        ReadBuffer readBuffer;

    public:
        using Base::Base;

    public:
        [[nodiscard]] BlobViewRet read() noexcept {
            BOOST_LEAF_AUTO(availableBytes, utils::getAvailableBytesForRead(Base::getNativeHandler()));
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

    class RS485 final : public detail::RS485Base {
    private:
        using Base = detail::RS485Base;

        using Base::read;

    public:
        using Base::Base;

        [[nodiscard]] BlobRet read() const noexcept;

        [[nodiscard]] BlobRet read(std::size_t size) const noexcept;
    };

    using ExtBufferRS485 = detail::RS485Base;
}