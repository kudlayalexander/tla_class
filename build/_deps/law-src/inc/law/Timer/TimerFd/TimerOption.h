#pragma once

#include "law/FlagValue.h"

namespace law::timer::tfd {
    struct TimerOption : public IntFlagValue {
        using IntFlagValue::IntFlagValue;

        TimerOption operator&(const TimerOption &co) const noexcept;

        TimerOption operator|(const TimerOption &co) const noexcept;
    };

    struct AbsTime : TimerOption {
        AbsTime() noexcept;
    };
}