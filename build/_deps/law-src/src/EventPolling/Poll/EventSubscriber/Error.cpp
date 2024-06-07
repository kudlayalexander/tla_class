#include <sys/poll.h>

#include "law/EventPolling/Poll/EventSubscriber/Error.h"

namespace law::ep::poll {
    Error Error::operator&(const Error &e) const noexcept {
        const auto ev = short(getAsIntegral() & e.getAsIntegral());
        return ev;
    }

    Error Error::operator|(const Error &e) const noexcept {
        const auto ev = short(getAsIntegral() | e.getAsIntegral());
        return ev;
    }

    ErrorCondition::ErrorCondition() noexcept: Error(POLLERR) {}

    HangUp::HangUp() noexcept: Error(POLLHUP) {}

    StreamSocketHangUp::StreamSocketHangUp() noexcept: Error(POLLRDHUP) {}
}