#pragma once

#include "law/FlagValue.h"

namespace law::ep::poll {
    struct Event : public ShortFlagValue {
        using ShortFlagValue::ShortFlagValue;

        Event operator&(const Event &e) const noexcept;

        Event operator|(const Event &e) const noexcept;
    };
    
    struct IncomingDataAvailable : Event {
        IncomingDataAvailable() noexcept;
    };

    struct UrgentDataAvailable : Event {
        UrgentDataAvailable() noexcept;
    };

    struct AvailableForWriting : Event {
        AvailableForWriting() noexcept;
    };

    struct PriorityDataAvailable : Event {
        PriorityDataAvailable() noexcept;
    };

    struct AvailableForPriorityWriting : Event {
        AvailableForPriorityWriting() noexcept;
    };
}