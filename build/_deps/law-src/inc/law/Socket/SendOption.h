#pragma once

#include "ReadSendOption.h"

namespace law::socket {
    struct SendOption : public ReadSendOption {
        using ReadSendOption::ReadSendOption;

        SendOption operator|(const SendOption &wo) const noexcept;
    };

    struct Confirm : SendOption {
        Confirm() noexcept;
    };

    struct DontRoute : SendOption {
        DontRoute() noexcept;
    };

    struct Eor : SendOption {
        Eor() noexcept;
    };

    struct More : SendOption {
        More() noexcept;
    };

    struct NoSignal : SendOption {
        NoSignal() noexcept;
    };
}