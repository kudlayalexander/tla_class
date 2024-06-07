#pragma once

#include <map>
#include <vector>

#include <etl/map.h>
#include <etl/vector.h>
#include <etl/multimap.h>

#include "Base/InotifyBase.h"

namespace law::fw::inotify {
    template <std::size_t maxSubs>
    using StaticInotify =
            detail::InotifyBase<
                    etl::vector<EventSubscriberObsPtr , maxSubs>,
                    etl::vector<EventSubscriberObsPtr , maxSubs>,
                    etl::multimap<int, EventSubscriberObsPtr, maxSubs>>;

    using Inotify =
            detail::InotifyBase<
                    std::vector<EventSubscriberObsPtr>,
                    std::vector<EventSubscriberObsPtr>,
                    std::multimap<int, EventSubscriberObsPtr>>;
}