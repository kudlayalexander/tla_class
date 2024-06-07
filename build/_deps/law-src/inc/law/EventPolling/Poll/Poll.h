#pragma once

#include <map>
#include <vector>

#include <etl/map.h>
#include <etl/vector.h>

#include "Base/PollBase.h"

namespace law::ep::poll {
    template <std::size_t maxSubs>
    using StaticPoll =
            detail::PollBase<
                    etl::vector<EventSubscriberObsPtr, maxSubs>,
                    etl::vector<EventSubscriberObsPtr, maxSubs>,
                    etl::map<int, EventSubscriberObsPtr, maxSubs>,
                    etl::vector<pollfd, maxSubs>
            >;

    using Poll =
            detail::PollBase<
                    std::vector<EventSubscriberObsPtr>,
                    std::vector<EventSubscriberObsPtr>,
                    std::map<int, EventSubscriberObsPtr>,
                    std::vector<pollfd>
            >;
}