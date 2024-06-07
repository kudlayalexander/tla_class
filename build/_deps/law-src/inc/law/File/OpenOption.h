#pragma once

#include "law/FlagValue.h"

namespace law::file {
    struct OpenOption : public IntFlagValue {
        using IntFlagValue::IntFlagValue;

        OpenOption operator&(const OpenOption &of) const noexcept;

        OpenOption operator|(const OpenOption &of) const noexcept;
    };

    struct Append final : OpenOption {
        Append() noexcept;
    };

    struct Async final : OpenOption {
        Async() noexcept;
    };

    struct CloseOnExec final : OpenOption {
        CloseOnExec() noexcept;
    };

    struct Create final : OpenOption {
        Create() noexcept;
    };

    struct Direct final : OpenOption {
        Direct() noexcept;
    };

    struct Directory final : OpenOption {
        Directory() noexcept;
    };

    struct Dsync final : OpenOption {
        Dsync() noexcept;
    };

    struct ExitIfExist final : OpenOption {
        ExitIfExist() noexcept;
    };

    struct LargeFile final : OpenOption {
        LargeFile() noexcept;
    };

    struct NoAtime final : OpenOption {
        NoAtime() noexcept;
    };

    struct NoCTty final : OpenOption {
        NoCTty() noexcept;
    };

    struct NoFollow final : OpenOption {
        NoFollow() noexcept;
    };

    struct Nonblock final : OpenOption {
        Nonblock() noexcept;
    };

    struct NoDelay final : OpenOption {
        NoDelay() noexcept;
    };

    struct Sync final : OpenOption {
        Sync() noexcept;
    };

    struct TempFile final : OpenOption {
        TempFile() noexcept;
    };

    struct Truncate final : OpenOption {
        Truncate() noexcept;
    };

    struct ReadOnly final : OpenOption {
        ReadOnly() noexcept;
    };

    struct WriteOnly final : OpenOption {
        WriteOnly() noexcept;
    };

    struct ReadWrite final : OpenOption {
        ReadWrite() noexcept;
    };
}