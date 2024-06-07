#pragma once

#include "ReadSendOption.h"

namespace law::socket {
    struct ReadOption : public ReadSendOption {
        using ReadSendOption::ReadSendOption;

        ReadOption operator|(const ReadOption &ro) const noexcept;
    };

    struct CmsgCloseOnExec : ReadOption {
        CmsgCloseOnExec() noexcept;
    };

    struct Peek : ReadOption {
        Peek() noexcept;
    };

    struct Truncate : ReadOption {
        Truncate() noexcept;
    };

    struct WaitAll : ReadOption {
        WaitAll() noexcept;
    };
}