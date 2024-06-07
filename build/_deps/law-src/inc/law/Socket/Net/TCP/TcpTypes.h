#pragma once

#include <functional>
#include <ErrorCode/ErrorCode.h>

namespace law::socket::net::tcp {
    using SocketFd = ErrorCode::CustomError<int>;

    using SocketError = ErrorCode::CustomError<std::uint32_t>;
}