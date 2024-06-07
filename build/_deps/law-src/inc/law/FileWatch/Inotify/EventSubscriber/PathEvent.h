#pragma once

#include "law/FlagValue.h"

namespace law::fw::inotify {
    struct PathEvent : public UInt32FlagValue {
        using UInt32FlagValue::UInt32FlagValue;

        PathEvent operator&(const PathEvent &fv) const noexcept;

        PathEvent operator|(const PathEvent &fv) const noexcept;
    };

    struct Access : PathEvent {
        Access() noexcept;
    };

    struct Modify : PathEvent {
        Modify() noexcept;
    };

    struct Attrib : PathEvent {
        Attrib() noexcept;
    };

    struct CloseWrite : PathEvent {
        CloseWrite() noexcept;
    };

    struct CloseNoWrite : PathEvent {
        CloseNoWrite() noexcept;
    };

    struct Close : PathEvent {
        Close() noexcept;
    };

    struct Open : PathEvent {
        Open() noexcept;
    };

    struct MovedFrom : PathEvent {
        MovedFrom() noexcept;
    };

    struct MovedTo : PathEvent {
        MovedTo() noexcept;
    };

    struct Move : PathEvent {
        Move() noexcept;
    };

    struct Create : PathEvent {
        Create() noexcept;
    };

    struct Delete : PathEvent {
        Delete() noexcept;
    };

    struct DeleteSelf : PathEvent {
        DeleteSelf() noexcept;
    };

    struct MoveSelf : PathEvent {
        MoveSelf() noexcept;
    };

    struct UnMount : PathEvent {
        UnMount() noexcept;
    };

    struct AllEvents : PathEvent {
        AllEvents() noexcept;
    };
}