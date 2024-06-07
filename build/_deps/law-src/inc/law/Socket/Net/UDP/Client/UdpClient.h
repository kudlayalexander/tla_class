#pragma once

#include "UdpClientBase.h"

namespace law::socket::net::udp {
    template <std::size_t bufSize>
    class StaticUdpClient final : public detail::UdpClientBase {
    private:
        using Base = detail::UdpClientBase;
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

    class UdpClient final : public detail::UdpClientBase {
    private:
        using Base = detail::UdpClientBase;

        using Base::read;

    public:
        using Base::Base;

        [[nodiscard]] BlobRet read(const ReadOption &ro = ReadOption(0)) const noexcept;

        [[nodiscard]] BlobRet read(std::size_t size, const ReadOption &ro = ReadOption(0)) const noexcept;
    };

    using ExtBufferUdpClient = detail::UdpClientBase;
}