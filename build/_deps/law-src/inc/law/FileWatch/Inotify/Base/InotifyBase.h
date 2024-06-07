#pragma once

#include <sys/inotify.h>

#include <Utils/Hash/CRC32/CRC32.h>

#include "law/RetCode/RetCode.h"
#include "law/FileWatch/Inotify/IInotify.h"
#include "law/FileDescriptor/FileDescriptor.h"

namespace law::fw::inotify::detail {
    template <typename AddListType, typename DeleteListType, typename SubsMapType>
    class InotifyBase : public IInotify {
    private:
        InitOption options;

        AddListType addList;
        DeleteListType deleteList;
        SubsMapType fileEventSubs;

        bool onEventHandling = false;

    public:
        InotifyBase() = default;

        explicit InotifyBase(InitOption options) :
                IInotify(inotify_init1(options.getAsIntegral())),
                options(options) {
            if (!this->operator bool()) {
                throw std::system_error(errno, std::system_category());
            }
        }

        ~InotifyBase() noexcept override {
            if (this->operator bool()) {
                for (const auto &[wfd, sub]: fileEventSubs) {
                    removeFileWatch(wfd);
                }
            }
        }

        Status init() noexcept override {
            return init(InitOption(0));
        }

        Status init(const InitOption &opts) noexcept override {
            auto fd = inotify_init1(opts.getAsIntegral());
            if (fd == -1) {
                return fromErrno();
            }

            options = opts;
            setNativeHandler(&fd);
            return {};
        }

        [[nodiscard]] InitOption getOptions() const noexcept override {
            return options;
        }

        Status subscribe(EventSubscriberObsPtr sub) noexcept override {
            if constexpr (law::detail::IsStaticContainer<AddListType>) {
                if (addList.full()) {
                    return boost::leaf::new_error(RetCode::NO_AVAILABLE_SPACE);
                }
            }

            if (!onEventHandling) {
                return addToWatch(sub);
            }

            if (std::ranges::find(addList, sub) == addList.end()) {
                addList.push_back(sub);
            }

            return {};
        }

        Status unsubscribe(EventSubscriberObsPtr sub) noexcept override {
            if constexpr (law::detail::IsStaticContainer<AddListType>) {
                if (deleteList.full()) {
                    return boost::leaf::new_error(RetCode::NO_AVAILABLE_SPACE);
                }
            }

            if (!onEventHandling) {
                return removeFromWatch(sub);
            }

            if (std::ranges::find(deleteList, sub) == deleteList.end()) {
                deleteList.push_back(sub);
            }

            return {};
        }

        Status poll() noexcept override {
            for (auto &&it: addList) {
                BOOST_LEAF_CHECK(addToWatch(it));
            }

            for (auto &&it: deleteList) {
                BOOST_LEAF_CHECK(removeFromWatch(it));
            }

            addList.clear();
            deleteList.clear();

            onEventHandling = true;
            BOOST_LEAF_CHECK(pollFileEvents());
            onEventHandling = false;

            return {};
        }

    protected:
        Status addToWatch(EventSubscriberObsPtr sub) noexcept {
            if constexpr (law::detail::IsStaticContainer<SubsMapType>) {
                if (fileEventSubs.full()) {
                    return boost::leaf::new_error(RetCode::NO_AVAILABLE_SPACE);
                }
            }

            const auto it = std::ranges::find_if(fileEventSubs,
                                                 [&sub](const auto &v) {
                                                     return (v.second == sub);
                                                 }
            );

            /* inotify_add_watch return the same wfd, if file already watch */
            BOOST_LEAF_AUTO(wfd, makeFileWatch(sub));
            if (it == fileEventSubs.cend()) {
                std::pair node(wfd, sub);
                fileEventSubs.insert(node);
            }

            return {};
        }

        Status removeFromWatch(EventSubscriberObsPtr sub) noexcept {
            const auto it = std::ranges::find_if(fileEventSubs,
                                                 [&sub](const auto &v) {
                                                     return (v.second == sub);
                                                 }
            );

            if (it != fileEventSubs.cend()) {
                removeFileWatch(it->first);
                fileEventSubs.erase(it);
            }

            return {};
        }

        Status pollFileEvents() noexcept {
            static char buf[4096] __attribute__((aligned(__alignof__(inotify_event)))) = {0};
            while (true) {
                auto len = ::read(this->getNativeHandler(), buf, sizeof(buf));
                if (len == -1) {
                    return (errno == EAGAIN) ? Status() : fromErrno();
                }

                const inotify_event *event;
                for (auto ptr = buf; ptr < (buf + len); ptr += (sizeof(inotify_event) + event->len)) {
                    event = reinterpret_cast<const inotify_event *>(ptr);

                    if (event->mask & IN_Q_OVERFLOW) {
                        continue;
                    }

                    auto range = fileEventSubs.equal_range(event->wd);
                    for (auto i = range.first; i != range.second; ++i) {
                        auto &sub = i->second;
                        if (event->mask & (IN_IGNORED | IN_UNMOUNT)) {
                            sub->onDelete(DeleteReason(event->mask));
                            unsubscribe(sub);
                        } else if (event->mask & sub->awaitableEvents().getAsIntegral()) {
                            PathEvent eventMask(event->mask);
                            const auto eventObjectName = event->len ? event->name : sub->watchPathname();
                            sub->onEvent(eventObjectName, eventMask);
                        }
                    }
                }
            }
        }

        void removeFileWatch(int wfd) const noexcept {
            inotify_rm_watch(this->getNativeHandler(), wfd);
        }

        [[nodiscard]] FdRet makeFileWatch(EventSubscriberObsPtr sub) const {
            std::uint32_t events = (sub->awaitableEvents().getAsIntegral() | sub->watchOptions().getAsIntegral());
            auto wd = inotify_add_watch(this->getNativeHandler(), sub->watchPathname().data(), events);
            if (wd == -1) {
                return fromErrno();
            }
            return wd;
        }
    };
}