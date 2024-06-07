#pragma once

#include "law/Socket/SendOption.h"
#include "law/Socket/ReadOption.h"
#include "law/Socket/Net/TCP/TcpTypes.h"
#include "law/FileDescriptor/FileDescriptor.h"

namespace law::socket::net::tcp::detail {
    struct TcpConnectionBase : public fd::FileDescriptor {
        explicit TcpConnectionBase(int fd) noexcept;

        Status send(std::string_view data, const SendOption &so = NoSignal()) const noexcept;

        [[nodiscard]] ReadCountRet read(void *extBuffer, std::size_t readCount, const ReadOption &ro = ReadOption(0)) const noexcept;

        Status destroy() noexcept;
    };
}