#pragma once

#include <span>
#include <experimental/memory>

#include "Event.h"
#include "Error.h"

namespace law::ep::poll {
    struct IEventSubscriber {
        virtual ~IEventSubscriber() noexcept = default;

        virtual void onEvent(Event ev) = 0;

        virtual void onError(const Error &) { /* implementation is optional */ }

        virtual void onDelete() { /* implementation is optional */ }

        [[nodiscard]] virtual int fileDescriptor() const noexcept = 0;

        [[nodiscard]] virtual Event awaitableEvents() const noexcept = 0;
    };

    /** For explicit non-owning pointer usage */
    using EventSubscriberObsPtr = std::experimental::observer_ptr<IEventSubscriber>;

    /** For explicit unique owning of a pointer with resource automatic free */
    using EventSubscriberUniqPtr = std::unique_ptr<IEventSubscriber>;

    /** For explicit shared owning of a pointer with resource automatic free */
    using EventSubscriberSharePtr = std::shared_ptr<IEventSubscriber>;

    /** For explicit shared non-owning of a pointer */
    using EventSubscriberWeakPtr = std::weak_ptr<IEventSubscriber>;
}