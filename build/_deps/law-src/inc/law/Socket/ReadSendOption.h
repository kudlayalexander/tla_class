#pragma once

#include "law/FlagValue.h"

namespace law::socket {
    struct ReadSendOption : public IntFlagValue {
        using IntFlagValue::IntFlagValue;

        ReadSendOption operator|(const ReadSendOption &rwo) const noexcept;
    };

    struct DontWait : ReadSendOption {
        DontWait() noexcept;
    };

    struct OutOfBand : ReadSendOption {
        OutOfBand() noexcept;
    };
}