#pragma once

#include "law/FlagValue.h"

namespace law::ep::poll {
    struct Error : public ShortFlagValue {
        using ShortFlagValue::ShortFlagValue;

        Error operator&(const Error &e) const noexcept;

        Error operator|(const Error &e) const noexcept;
    };

    struct ErrorCondition : Error {
        ErrorCondition() noexcept;
    };

    struct HangUp : Error {
        HangUp() noexcept;
    };

    struct StreamSocketHangUp : Error {
        StreamSocketHangUp() noexcept;
    };
}