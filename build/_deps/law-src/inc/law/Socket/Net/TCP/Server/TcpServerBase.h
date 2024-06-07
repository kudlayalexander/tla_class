#pragma once

#include "AcceptOption.h"
#include "law/Socket/SendOption.h"
#include "law/Socket/Net/TCP/Base/TcpSocketBase.h"
#include "law/Socket/Net/InternetAddress/InternetAddress.h"

namespace law::socket::net::tcp::detail {
    class TcpServerBase : public TcpSocketBase {
    private:
        using Base = TcpSocketBase;

    private:
        InternetAddress address{};

    public:
        TcpServerBase() noexcept = default;

        TcpServerBase(InternetAddress &&addr);

        TcpServerBase(const InternetAddress &addr);

        Status start(std::uint16_t maxConnectionInQueue = SOMAXCONN) const noexcept;

    protected:
        [[nodiscard]] SocketFd acceptConnection(const AcceptOption &ao = AcceptOption(0)) const noexcept;
    };
}
