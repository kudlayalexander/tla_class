#include <sys/inotify.h>

#include "law/FileWatch/Inotify/EventSubscriber/WatchOption.h"

namespace law::fw::inotify {
    WatchOption WatchOption::operator&(const WatchOption &fv) const noexcept {
        return {getAsIntegral() & fv.getAsIntegral()};
    }

    WatchOption WatchOption::operator|(const WatchOption &fv) const noexcept {
        return {getAsIntegral() | fv.getAsIntegral()};
    }

    OnlyDir::OnlyDir() noexcept: WatchOption(IN_ONLYDIR) {}

    DontFollow::DontFollow() noexcept: WatchOption(IN_DONT_FOLLOW) {}

    ExclUnlink::ExclUnlink() noexcept: WatchOption(IN_EXCL_UNLINK) {}

    MaskAdd::MaskAdd() noexcept: WatchOption(IN_MASK_ADD) {}

    Oneshot::Oneshot() noexcept: WatchOption(IN_ONESHOT) {}
}