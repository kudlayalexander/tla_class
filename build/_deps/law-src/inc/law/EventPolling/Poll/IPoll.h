#pragma once

#include <chrono>

#include "law/Types.h"
#include "EventSubscriber/IEventSubscriber.h"

namespace law::ep::poll {
    struct IPoll {
        virtual ~IPoll() noexcept = default;

        virtual Status subscribe(EventSubscriberObsPtr sub) noexcept = 0;

        virtual Status unsubscribe(EventSubscriberObsPtr sub) noexcept = 0;

        virtual Status poll() noexcept = 0;

        virtual Status poll(std::chrono::milliseconds ms) noexcept = 0;
    };

    /** For explicit non-owning pointer usage */
    using PollObsPtr = std::experimental::observer_ptr<IPoll>;

    /** For explicit unique owning of a pointer with resource automatic free */
    using PollUniqPtr = std::unique_ptr<IPoll>;

    /** For explicit shared owning of a pointer with resource automatic free */
    using PollSharePtr = std::shared_ptr<IPoll>;

    /** For explicit shared non-owning of a pointer */
    using PollWeakPtr = std::weak_ptr<IPoll>;
}