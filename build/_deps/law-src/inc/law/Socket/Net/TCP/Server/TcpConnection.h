#pragma once

#include <array>

#include "law/Utils.h"
#include "TcpConnectionBase.h"
#include "law/RetCode/RetCode.h"

namespace law::socket::net::tcp {
    template <std::size_t bufSize>
    class StaticTcpConnection final : public detail::TcpConnectionBase {
    private:
        using Base = detail::TcpConnectionBase;
        using ReadBuffer = std::array<char, bufSize + 1>;

    private:
        static constexpr std::uint8_t kDefaultReadCount = bufSize;

    private:
        ReadBuffer readBuffer;

    public:
        using Base::Base;

        [[nodiscard]] BlobViewRet read(const ReadOption &ro = ReadOption(0)) noexcept {
            BOOST_LEAF_AUTO(availableBytes, utils::getAvailableBytesForRead(getNativeHandler()));
            return (
                    (availableBytes == 0) ?
                    read(kDefaultReadCount, ro) :
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

    class TcpConnection final : public detail::TcpConnectionBase {
    private:
        using Base = detail::TcpConnectionBase;

    private:
        static constexpr std::uint8_t kDefaultReadCount = UINT8_MAX;

    public:
        using Base::Base;

        [[nodiscard]] BlobRet read(const ReadOption &ro = ReadOption(0)) noexcept;

        [[nodiscard]] BlobRet read(std::size_t size, const ReadOption &ro = ReadOption(0)) noexcept;
    };

    using ExtBufferTcpConnection = detail::TcpConnectionBase;
}