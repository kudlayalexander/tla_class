#include <sys/poll.h>

#include "law/EventPolling/Poll/EventSubscriber/Event.h"

namespace law::ep::poll {
    Event Event::operator&(const Event &e) const noexcept {
        const auto ev = short(getAsIntegral() & e.getAsIntegral());
        return ev;
    }

    Event Event::operator|(const Event &e) const noexcept {
        const auto ev = short(getAsIntegral() | e.getAsIntegral());
        return ev;
    }

    IncomingDataAvailable::IncomingDataAvailable() noexcept: Event(POLLIN) {}

    UrgentDataAvailable::UrgentDataAvailable() noexcept: Event(POLLPRI) {}

    AvailableForWriting::AvailableForWriting() noexcept: Event(POLLOUT) {}

    PriorityDataAvailable::PriorityDataAvailable() noexcept: Event(POLLRDBAND) {}

    AvailableForPriorityWriting::AvailableForPriorityWriting() noexcept: Event(POLLWRBAND) {}
}