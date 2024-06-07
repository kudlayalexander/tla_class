#pragma once

#include <array>

#include "law/Utils.h"
#include "UdsUdpServerBase.h"
#include "law/Socket/Unix/UDP/UdsUdpTypes.h"

namespace law::socket::uds::udp {
    template <std::size_t bufSize>
    class StaticUdsUdpServer final : public detail::UdsUdpServerBase {
    private:
        using Base = detail::UdsUdpServerBase;
        using ReadBuffer = std::array<char, bufSize + 1>;

        using Base::read;

    private:
        ReadBuffer readBuffer;

    public:
        using Base::Base;

        [[nodiscard]] ClientPayloadViewRet read(const ReadOption &ro = ReadOption(0)) noexcept {
            BOOST_LEAF_AUTO(availableBytes, utils::getAvailableBytesForRead(getNativeHandler()));
            return (
                    (availableBytes == 0) ?
                    read(getDefaultReadCount(), ro) :
                    read((availableBytes > bufSize ? bufSize : availableBytes), ro)
            );
        }

        [[nodiscard]] ClientPayloadViewRet
        read(std::size_t size, const ReadOption &ro = ReadOption(0)) noexcept {
            if (size > bufSize) {
                return boost::leaf::new_error(RetCode::NO_AVAILABLE_SPACE);
            }

            BOOST_LEAF_AUTO(readResult, Base::read(&readBuffer[0], size, ro));
            auto &[clientAddress, recBytes] = readResult;

            readBuffer[recBytes] = '\0';
            return makeClientPayload(clientAddress, recBytes);
        }

    private:
        [[nodiscard]] ClientPayloadView makeClientPayload(const sockaddr_un &nativeAddress, std::size_t recBytes) const noexcept {
            ClientPayloadView ret;
            ret.address.setNativeAddress(nativeAddress);
            ret.blob = {readBuffer.data(), recBytes};
            return ret;
        }
    };

    class UdsUdpServer final : public detail::UdsUdpServerBase {
    private:
        using Base = detail::UdsUdpServerBase;

        using Base::read;

    public:
        using Base::Base;

        [[nodiscard]] ClientPayloadRet read(const ReadOption &ro = ReadOption(0)) noexcept;

        [[nodiscard]] ClientPayloadRet read(std::size_t size, const ReadOption &ro = ReadOption(0)) const noexcept;

    private:
        [[nodiscard]] static ClientPayload makeClientPayload(const sockaddr_un &nativeAddress, std::string &&payload) noexcept;
    };

    using ExtBufferUdsUdpServer = detail::UdsUdpServerBase;
}