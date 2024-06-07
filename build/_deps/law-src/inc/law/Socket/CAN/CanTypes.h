#pragma once

#include <span>

#include <linux/can.h>

#include <etl/string.h>

#include "law/Types.h"

namespace law::socket::can {
    enum class CanMode : std::uint8_t {
        CAN = CAN_MTU,
        CAN_FD = CANFD_MTU
    };

    struct CanFrame {
        std::uint32_t id = 0;
        etl::string<CANFD_MAX_DLEN> data{};
    };

    using CanFrameSequence = std::span<CanFrame>;
    using CanFrameRet = ErrorCode::CustomError<CanFrame>;

    using CanFilter = can_filter;
    using CanFilterSequence = std::span<CanFilter>;
}