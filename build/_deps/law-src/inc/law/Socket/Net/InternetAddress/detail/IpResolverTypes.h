#pragma once

#include <netinet/in.h>

#include <ErrorCode/ErrorCode.h>

namespace law::socket::net::detail {
    using ResolvedIP = ErrorCode::CustomError<in_addr>;
}