#pragma once

#include <string_view>
#include <experimental/memory>

#include "PathEvent.h"
#include "WatchOption.h"
#include "DeleteReason.h"

namespace law::fw::inotify {
    struct IEventSubscriber {
        virtual ~IEventSubscriber() noexcept = default;

        virtual void onEvent(std::string_view filename, PathEvent fe) = 0;

        virtual void onDelete(const DeleteReason &) { /* implementation is optional */ }

        [[nodiscard]] virtual std::string_view watchPathname() const noexcept = 0;

        [[nodiscard]] virtual PathEvent awaitableEvents() const noexcept = 0;

        [[nodiscard]] virtual WatchOption watchOptions() const noexcept {
            return 0;
        }
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