#pragma once

#include <span>

#include <linux/can/j1939.h>

#include <ErrorCode/ErrorCode.h>

namespace law::socket::can {
    using J1939Filter = j1939_filter;
    using J1939FilterSequence = std::span<J1939Filter>;

    enum class J1939SendPriority : std::uint8_t {
        PRIO_0 = 0,
        PRIO_1 = 1,
        PRIO_2 = 2,
        PRIO_3 = 3,
        PRIO_4 = 4,
        PRIO_5 = 5,
        PRIO_6 = 6,
        PRIO_7 = 7,
    };

    using J1939SendPriorityRet = ErrorCode::CustomError<J1939SendPriority>;

    struct ReadResult {
        sockaddr_can sourceAddress;
        std::size_t recBytes;
    };

    using ReadResultRet = ErrorCode::CustomError<ReadResult>;

    struct ReadResultBlobView {
        sockaddr_can sourceAddress;
        std::string_view blob;
    };

    struct ReadResultBlob {
        sockaddr_can sourceAddress;
        std::string blob;
    };

    using ReadResultBlobViewRet = ErrorCode::CustomError<ReadResultBlobView>;
    using ReadResultBlobRet = ErrorCode::CustomError<ReadResultBlob>;
}