#pragma once

#include "law/FlagValue.h"

namespace law::fw::inotify {
    struct DeleteReason : public UInt32FlagValue {
        using UInt32FlagValue::UInt32FlagValue;

        DeleteReason operator&(const DeleteReason &fv) const noexcept;
    };

    struct WatchPathnameRemoved : DeleteReason {
        WatchPathnameRemoved() noexcept;
    };

    struct WatchPathnameFsUnmount : DeleteReason {
        WatchPathnameFsUnmount() noexcept;
    };
}