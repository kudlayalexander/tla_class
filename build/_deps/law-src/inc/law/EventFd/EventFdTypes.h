#pragma once

#include <sys/eventfd.h>

#include <ErrorCode/ErrorCode.h>

namespace law::efd {
    /** Just alias for glibc eventfd_t type */
    using Value = eventfd_t;

    using ValueRet = ErrorCode::CustomError<eventfd_t>;
}