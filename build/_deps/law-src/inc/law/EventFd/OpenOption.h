#pragma once

#include "law/FlagValue.h"

namespace law::efd {
    struct OpenOption : IntFlagValue {
        using IntFlagValue::IntFlagValue;

        OpenOption operator|(const OpenOption &rhs) const noexcept;

        OpenOption operator&(const OpenOption &rhs) const noexcept;
    };

    struct CloseOnExec : OpenOption {
        CloseOnExec() noexcept;
    };

    struct Nonblock : OpenOption {
        Nonblock() noexcept;
    };

    struct Semaphore : OpenOption {
        Semaphore() noexcept;
    };
}