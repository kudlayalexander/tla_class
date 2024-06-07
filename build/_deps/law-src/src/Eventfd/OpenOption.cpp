#include <sys/eventfd.h>

#include "law/EventFd/OpenOption.h"

namespace law::efd {
    OpenOption OpenOption::operator|(const OpenOption &rhs) const noexcept {
        return {getAsIntegral() | rhs.getAsIntegral()};
    }

    OpenOption OpenOption::operator&(const OpenOption &rhs) const noexcept {
        return {getAsIntegral() & rhs.getAsIntegral()};
    }

    CloseOnExec::CloseOnExec() noexcept: OpenOption(EFD_CLOEXEC) {}

    Nonblock::Nonblock() noexcept: OpenOption(EFD_NONBLOCK) {}

    Semaphore::Semaphore() noexcept: OpenOption(EFD_SEMAPHORE) {}
}