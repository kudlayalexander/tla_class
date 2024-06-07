#pragma once

#include <ErrorCode/ErrorCode.h>

namespace law::pipe {
    enum class PipeType : std::uint8_t {
        READ,
        WRITE,
        READ_WRITE
    };

    using PipeSize = ErrorCode::CustomError<std::size_t>;
}