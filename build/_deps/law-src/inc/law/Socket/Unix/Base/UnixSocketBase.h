#pragma once

#include "law/Socket/Base/SocketType.h"
#include "law/Socket/Base/SocketBase.h"

namespace law::socket::uds::detail {
    class UnixSocketBase : public socket::detail::SocketBase {
    private:
        using socket::detail::SocketBase::makeSocket;

    protected:
        Status makeSocket(socket::detail::SocketType socketType) noexcept;
    };
}