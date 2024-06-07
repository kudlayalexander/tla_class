#pragma once

#include "InitOption.h"
#include "EventSubscriber/IEventSubscriber.h"
#include "law/FileDescriptor/FileDescriptor.h"

namespace law::fw::inotify {
    struct IInotify : public fd::FileDescriptor {
        using fd::FileDescriptor::FileDescriptor;

        virtual Status init(const InitOption &opts) noexcept = 0;

        virtual Status init() noexcept = 0;

        [[nodiscard]] virtual InitOption getOptions() const noexcept = 0;

        virtual Status subscribe(EventSubscriberObsPtr sub) noexcept = 0;

        virtual Status unsubscribe(EventSubscriberObsPtr sub) noexcept = 0;

        virtual Status poll() noexcept = 0;
    };

    /** For explicit non-owning pointer usage */
    using InotifyObsPtr = std::experimental::observer_ptr<IInotify>;

    /** For explicit unique owning of a pointer with resource automatic free */
    using InotifyUniqPtr = std::unique_ptr<IInotify>;

    /** For explicit shared owning of a pointer with resource automatic free */
    using InotifySharePtr = std::shared_ptr<IInotify>;

    /** For explicit shared non-owning of a pointer */
    using InotifyWeakPtr = std::weak_ptr<IInotify>;
}