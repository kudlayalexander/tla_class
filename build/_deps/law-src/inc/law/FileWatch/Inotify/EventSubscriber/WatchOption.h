#pragma once

#include "law/FlagValue.h"

namespace law::fw::inotify {
    struct WatchOption : public UInt32FlagValue {
        using UInt32FlagValue::UInt32FlagValue;

        WatchOption operator&(const WatchOption &fv) const noexcept;

        WatchOption operator|(const WatchOption &fv) const noexcept;
    };

    struct OnlyDir : WatchOption {
        OnlyDir() noexcept;
    };

    struct DontFollow : WatchOption {
        DontFollow() noexcept;
    };

    struct ExclUnlink : WatchOption {
        ExclUnlink() noexcept;
    };

    struct MaskAdd : WatchOption {
        MaskAdd() noexcept;
    };

    struct Oneshot : WatchOption {
        Oneshot() noexcept;
    };
}