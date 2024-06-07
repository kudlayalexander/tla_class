#pragma once

#include <map>
#include <list>
#include <csignal>
#include <functional>

#include <ErrorCode/ErrorCode.h>

namespace law::signal {
    using OnSignalCallback = std::function<void(int, siginfo_t *, void *)>;

    class SignalHandler {
    private:
        using SignalList = std::list<OnSignalCallback>;

    private:
        struct sigaction act{};
        std::map<int, SignalList> callbacks;

    private:
        SignalHandler() noexcept;

    public:
        SignalHandler(SignalHandler &&) = delete;

        SignalHandler(const SignalHandler &) = delete;

        SignalHandler &operator=(SignalHandler &&) = delete;

        SignalHandler &operator=(const SignalHandler &) = delete;

        static SignalHandler &getInstance() noexcept;

        std::size_t getSignalSubscribersCount(int signal) noexcept;

        template <typename ...Args>
        void subscribe(const OnSignalCallback &cb, Args ...signals) {
            (insertSignal(cb, signals), ...);
            (sigaddset(&act.sa_mask, signals), ...);
            for (const auto &[signum, callback]: callbacks) {
                sigaction(signum, &act, nullptr);
            }
        }

    private:
        static void signalHandler(int signum, siginfo_t *info, void *ucontext) noexcept;

        void insertSignal(const OnSignalCallback &cb, int signum);
    };
}