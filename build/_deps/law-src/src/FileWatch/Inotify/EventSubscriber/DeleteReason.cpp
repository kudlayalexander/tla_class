#include <sys/inotify.h>

#include "law/FileWatch/Inotify/EventSubscriber/DeleteReason.h"

namespace law::fw::inotify {
    DeleteReason DeleteReason::operator&(const DeleteReason &fv) const noexcept {
        return {getAsIntegral() & fv.getAsIntegral()};
    }

    WatchPathnameRemoved::WatchPathnameRemoved() noexcept: DeleteReason(IN_IGNORED) { }

    WatchPathnameFsUnmount::WatchPathnameFsUnmount() noexcept: DeleteReason(IN_UNMOUNT) { }
}