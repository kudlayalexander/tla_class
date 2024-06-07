#pragma once

#include "law/Socket/Net/Base/NetSocketBase.h"

namespace law::socket::net::udp::detail {
    class UdpSocketBase : public net::detail::NetSocketBase {
    private:
        using Base = net::detail::NetSocketBase;

    protected:
        void openSocket();

        Status makeSocket() noexcept;
    };
}