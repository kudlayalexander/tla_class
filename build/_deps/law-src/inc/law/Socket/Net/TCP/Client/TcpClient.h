#pragma once

#include <array>

#include "TcpClientBase.h"
#include "law/RetCode/RetCode.h"
#include "law/Socket/ReadOption.h"

namespace law::socket::net::tcp {
    template <std::size_t bufSize>
    class StaticTcpClient final : public detail::TcpClientBase {
    private:
        using Base = detail::TcpClientBase;
        using ReadBuffer = std::array<char, bufSize + 1>;

        using Base::read;

    private:
        ReadBuffer readBuffer;

    public:
        using Base::Base;

        [[nodiscard]] BlobViewRet read(const ReadOption &ro = ReadOption(0)) noexcept {
            BOOST_LEAF_AUTO(availableBytes, utils::getAvailableBytesForRead(getNativeHandler()));
            return (
                    (availableBytes == 0) ?
                    read(getDefaultReadCount(), ro) :
                    read((availableBytes > bufSize ? bufSize : availableBytes), ro)
            );
        }

        [[nodiscard]] BlobViewRet read(std::size_t size, const ReadOption &ro = ReadOption(0)) noexcept {
            if (size > bufSize) {
                return boost::leaf::new_error(RetCode::NO_AVAILABLE_SPACE);
            }

            BOOST_LEAF_AUTO(recBytes, Base::read(&readBuffer[0], size, ro.getAsIntegral()));

            readBuffer[recBytes] = '\0';
            return std::string_view{readBuffer.data(), std::string_view::size_type(recBytes)};
        }
    };

    class TcpClient final : public detail::TcpClientBase {
    private:
        using Base = detail::TcpClientBase;

        using Base::read;

    public:
        using Base::Base;

        [[nodiscard]] BlobRet read(const ReadOption &ro = ReadOption(0)) const noexcept;

        [[nodiscard]] BlobRet read(std::size_t size, const ReadOption &ro = ReadOption(0)) const noexcept;
    };

    using ExtBufferTcpClient = detail::TcpClientBase;
}