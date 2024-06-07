#include <sys/inotify.h>

#include "law/FileWatch/Inotify/EventSubscriber/PathEvent.h"

namespace law::fw::inotify {
    PathEvent PathEvent::operator&(const PathEvent &fv) const noexcept {
        return {getAsIntegral() & fv.getAsIntegral()};
    }

    PathEvent PathEvent::operator|(const PathEvent &fv) const noexcept {
        return {getAsIntegral() | fv.getAsIntegral()};
    }

    Access::Access() noexcept: PathEvent(IN_ACCESS) {}

    Modify::Modify() noexcept: PathEvent(IN_MODIFY) {}

    Attrib::Attrib() noexcept: PathEvent(IN_ATTRIB) {}

    CloseWrite::CloseWrite() noexcept: PathEvent(IN_CLOSE_WRITE) {}

    CloseNoWrite::CloseNoWrite() noexcept: PathEvent(IN_CLOSE_NOWRITE) {}

    Close::Close() noexcept: PathEvent(IN_CLOSE) {}

    Open::Open() noexcept: PathEvent(IN_OPEN) {}

    MovedFrom::MovedFrom() noexcept: PathEvent(IN_MOVED_FROM) {}

    MovedTo::MovedTo() noexcept: PathEvent(IN_MOVED_TO) {}

    Move::Move() noexcept: PathEvent(IN_MOVE) {}

    Create::Create() noexcept: PathEvent(IN_CREATE) {}

    Delete::Delete() noexcept: PathEvent(IN_DELETE) {}

    DeleteSelf::DeleteSelf() noexcept: PathEvent(IN_DELETE_SELF) {}

    MoveSelf::MoveSelf() noexcept: PathEvent(IN_MOVE_SELF) {}

    UnMount::UnMount() noexcept: PathEvent(IN_UNMOUNT) {}

    AllEvents::AllEvents() noexcept: PathEvent(IN_ALL_EVENTS) {}
}