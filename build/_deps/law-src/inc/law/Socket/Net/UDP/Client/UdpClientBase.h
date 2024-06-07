#pragma once

#include "law/Socket/SendOption.h"
#include "law/Socket/ReadOption.h"
#include "law/Socket/Net/UDP/Base/UdpSocketBase.h"
#include "law/Socket/Net/InternetAddress/InternetAddress.h"

namespace law::socket::net::udp::detail {
    class UdpClientBase : public detail::UdpSocketBase {
    public:
        using Base = detail::UdpSocketBase;

    private:
        InternetAddress serverAddress;

    public:
        UdpClientBase();

        explicit UdpClientBase(const InternetAddress &srvAddr);

        void setServerAddress(const InternetAddress &addr) noexcept;

        [[nodiscard]] const InternetAddress &getServerAddress() const noexcept;

        Status send(std::string_view data, const SendOption &so = SendOption(0)) const noexcept;

        ReadCountRet read(void *extBuffer, std::size_t size, const ReadOption &ro = ReadOption(0)) const noexcept;
    };
}