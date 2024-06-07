#pragma once

#include <ranges>
#include <chrono>
#include <algorithm>

#include <sys/poll.h>

#include "law/EventPolling/Poll/IPoll.h"

#include "law/Types.h"
#include "law/RetCode/RetCode.h"
#include "law/FileDescriptor/FileDescriptor.h"

namespace law::ep::poll::detail {
    template <typename AddListType, typename DeleteListType, typename SubsMapType, typename PollfdVector>
    class PollBase final : public IPoll {
    private:
        PollfdVector pfdVector;

        AddListType addList;
        DeleteListType deleteList;

        SubsMapType eventSubs;

        bool onEventHandling = false;

    public:
        Status subscribe(EventSubscriberObsPtr sub) noexcept override {
            if (sub->fileDescriptor() == -1) {
                return boost::leaf::new_error(RetCode::INVALID_FILE_DESCRIPTOR);
            }

            if constexpr (law::detail::IsStaticContainer<AddListType>) {
                if (addList.full()) {
                    return boost::leaf::new_error(RetCode::NO_AVAILABLE_SPACE);
                }
            }

            if (!onEventHandling) {
                return addToPolling(sub);
            }

            if (std::ranges::find(addList, sub) == addList.end()) {
                addList.push_back(sub);
            }

            return {};
        }

        Status unsubscribe(EventSubscriberObsPtr sub) noexcept override {
            if (sub->fileDescriptor() == -1) {
                return boost::leaf::new_error(RetCode::INVALID_FILE_DESCRIPTOR);
            }

            if constexpr (law::detail::IsStaticContainer<AddListType>) {
                if (deleteList.full()) {
                    return boost::leaf::new_error(RetCode::NO_AVAILABLE_SPACE);
                }
            }

            if (!onEventHandling) {
                return removeFromPolling(sub);
            }

            if (std::ranges::find(deleteList, sub) == deleteList.end()) {
                deleteList.push_back(sub);
            }

            return {};
        }

        Status poll() noexcept override {
            return poll(std::chrono::milliseconds::max());
        }

        Status poll(std::chrono::milliseconds ms) noexcept override {
            return boost::leaf::try_handle_some(
                    [this, ms]() -> Status {
                        for (auto &&it: addList) {
                            BOOST_LEAF_CHECK(addToPolling(it));
                        }

                        for (auto &&it: deleteList) {
                            BOOST_LEAF_CHECK(removeFromPolling(it));
                        }

                        addList.clear();
                        deleteList.clear();

                        onEventHandling = true;
                        BOOST_LEAF_CHECK(pollEvents((ms == std::chrono::milliseconds::max()) ? -1 : ms.count()));
                        onEventHandling = false;

                        return {};
                    }
            );
        }

    private:
        Status addToPolling(EventSubscriberObsPtr sub) noexcept {
            if constexpr (law::detail::IsStaticContainer<SubsMapType>) {
                if (eventSubs.full()) {
                    return boost::leaf::new_error(RetCode::NO_AVAILABLE_SPACE);
                }
            }

            auto it = std::ranges::find_if(
                    eventSubs,
                    [&sub](const auto &v) {
                        return (v.second == sub);
                    }
            );

            if (it != eventSubs.end()) {
                auto pfdEntry = findPfdEntryByFd(it->first);
                pfdEntry->events = sub->awaitableEvents().getAsIntegral();
            } else {
                eventSubs[sub->fileDescriptor()] = sub;
                pfdVector.push_back({sub->fileDescriptor(), sub->awaitableEvents().getAsIntegral(), 0});
            }

            return {};
        }

        Status removeFromPolling(EventSubscriberObsPtr sub) noexcept {
            const auto it = std::ranges::find_if(eventSubs,
                                                 [&sub](const auto &v) {
                                                     return (v.second == sub);
                                                 }
            );

            if (it != eventSubs.cend()) {
                const auto fd = it->first;
                pfdVector.erase(findPfdEntryByFd(fd));
                eventSubs.erase(it);
            }

            return {};
        }

        Status pollEvents(int ms) noexcept {
            switch (::poll(pfdVector.data(), pfdVector.size(), ms)) {
                case -1:
                    return ((errno != EINTR) ? fromErrno() : pollEvents(ms));
                case 0:
                    return boost::leaf::new_error(RetCode::TIMEOUT);
                default:
                    handleIncomingEvents();
                    clearIncomingEvents();
                    return {};
            }
        }

        typename PollfdVector::iterator findPfdEntryByFd(int fd) {
            auto pfdEntry = std::ranges::find_if(pfdVector,
                                                 [fd](const auto &pfd) {
                                                     return pfd.fd == fd;
                                                 });
            return pfdEntry;
        }

        void handleIncomingEvents() {
            for (const auto &it: pfdVector) {
                if (it.revents == 0) {
                    continue;
                }

                auto &sub = eventSubs[it.fd];
                if (it.revents & POLLNVAL) {
                    sub->onDelete();
                    removeFromPolling(sub);
                }

                if (it.revents & (POLLERR | POLLHUP | POLLRDHUP)) {
                    sub->onError(Error(it.revents));
                }

                if (it.revents & (POLLIN | POLLPRI | POLLOUT | POLLRDNORM | POLLRDBAND | POLLWRNORM | POLLWRBAND)) {
                    sub->onEvent(Event(it.revents));
                }
            }
        }

        void clearIncomingEvents() noexcept {
            for (auto &it: pfdVector) {
                it.revents = 0;
            }
        }
    };
}