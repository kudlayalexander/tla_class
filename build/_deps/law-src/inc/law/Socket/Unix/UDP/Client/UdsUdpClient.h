#pragma once

#include <array>

#include "law/Utils.h"
#include "law/RetCode/RetCode.h"
#include "law/Socket/Unix/UDP/Client/UdsUdpClientBase.h"

namespace law::socket::uds::udp {
    template <std::size_t bufSize>
    class StaticUdsUdpClient final : public detail::UdsUdpClientBase {
    private:
        using Base = detail::UdsUdpClientBase;
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

            BOOST_LEAF_AUTO(recBytes, Base::read(&readBuffer[0], size, ro));

            readBuffer[recBytes] = '\0';
            return std::string_view{readBuffer.data(), recBytes};
        }
    };

    class UdsUdpClient final : public detail::UdsUdpClientBase {
    private:
        using Base = detail::UdsUdpClientBase;

        using Base::read;

    public:
        using Base::Base;

        [[nodiscard]] BlobRet read(const ReadOption &ro = ReadOption(0)) const noexcept;

        [[nodiscard]] BlobRet read(std::size_t size, const ReadOption &ro = ReadOption(0)) const noexcept;
    };

    using ExtBufferUdsUdpClient = detail::UdsUdpClientBase;
}