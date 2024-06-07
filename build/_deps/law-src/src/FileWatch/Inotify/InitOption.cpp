#include <sys/inotify.h>

#include "law/FileWatch/Inotify/InitOption.h"

namespace law::fw::inotify {
    InitOption InitOption::operator|(const InitOption &fv) const noexcept {
        return {getAsIntegral() | fv.getAsIntegral()};
    }

    Nonblock::Nonblock() noexcept: InitOption(IN_NONBLOCK) {}

    CloseOnExec::CloseOnExec() noexcept: InitOption(IN_CLOEXEC) {}
}