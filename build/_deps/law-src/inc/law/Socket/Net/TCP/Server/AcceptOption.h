#pragma once

#include "law/FlagValue.h"

namespace law::socket::net::tcp {
    struct AcceptOption : public IntFlagValue {
        using IntFlagValue::IntFlagValue;

        AcceptOption operator|(AcceptOption ao) const noexcept;
    };

    struct Nonblock : AcceptOption {
        Nonblock() noexcept;
    };

    struct CloseOnExec : AcceptOption {
        CloseOnExec() noexcept;
    };
}