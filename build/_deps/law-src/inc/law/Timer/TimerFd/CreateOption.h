#pragma once

#include "law/FlagValue.h"

namespace law::timer::tfd {
    struct CreateOption : public IntFlagValue {
        using IntFlagValue::IntFlagValue;

        CreateOption operator&(const CreateOption &co) const noexcept;

        CreateOption operator|(const CreateOption &co) const noexcept;
    };

    struct Nonblock : CreateOption {
        Nonblock() noexcept;
    };

    struct CloseOnExec : CreateOption {
        CloseOnExec() noexcept;
    };
}