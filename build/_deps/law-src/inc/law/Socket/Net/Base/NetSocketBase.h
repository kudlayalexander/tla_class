#pragma once

#include "law/Socket/Base/SocketBase.h"

namespace law::socket::net::detail {
    class NetSocketBase : public socket::detail::SocketBase {
    private:
        using Base = socket::detail::SocketBase;
        using Base::makeSocket;

    protected:
        Status makeSocket(socket::detail::SocketType socketType) noexcept;
    };
}
