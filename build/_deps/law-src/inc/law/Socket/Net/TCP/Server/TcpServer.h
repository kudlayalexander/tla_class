#pragma once

#include <array>

#include "TcpConnection.h"
#include "TcpServerAcceptor.h"
#include "law/RetCode/RetCode.h"
#include "law/Socket/ReadOption.h"

namespace law::socket::net::tcp {
    template <std::size_t bufSize>
    using StaticTcpServer = detail::TcpServerAcceptor<StaticTcpConnection<bufSize>>;

    using TcpServer = detail::TcpServerAcceptor<TcpConnection>;

    using ExtBufferTcpServer = detail::TcpServerAcceptor<ExtBufferTcpConnection>;
}
