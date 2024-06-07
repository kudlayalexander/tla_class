#pragma once

#include "law/Socket/ReadOption.h"
#include "law/Socket/Unix/UnixAddress/UnixAddress.h"
#include "law/Socket/Unix/UDP/Base/UdsUdpSocketBase.h"

namespace law::socket::uds::udp::detail {
    class UdsUdpClientBase : public detail::UdsUdpSocketBase {
    public:
        using ReadBytesCount = std::size_t;

    private:
        using detail::UdsUdpSocketBase::send;

    private:
        UnixAddress selfAddress;
        UnixAddress serverAddress;

    public:
        UdsUdpClientBase();

        ~UdsUdpClientBase() noexcept override;

        explicit UdsUdpClientBase(const UnixAddress &srvAddr);

        void setServerAddress(const UnixAddress &addr) noexcept;

        [[nodiscard]] const UnixAddress &getServerAddress() const noexcept;

        Status send(std::string_view data) const noexcept;

        ReadCountRet read(void *extBuffer, std::size_t size, const ReadOption &ro = ReadOption(0)) const noexcept;

    private:
        void openSocket();
    };
}