#pragma once

#include "law/FlagValue.h"

namespace law::fw::inotify {
    struct InitOption : public IntFlagValue {
        using IntFlagValue::IntFlagValue;

        InitOption operator|(const InitOption &fv) const noexcept;
    };

    struct Nonblock : InitOption {
        Nonblock() noexcept;
    };

    struct CloseOnExec : InitOption {
        CloseOnExec() noexcept;
    };
}