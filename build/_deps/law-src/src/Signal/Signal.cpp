#include <iostream>

#include "law/Signal/Signal.h"

namespace law::signal {
    SignalHandler::SignalHandler() noexcept {
        std::memset(&act, 0, sizeof(act));
        act.sa_flags = SA_SIGINFO;
        act.sa_sigaction = signalHandler;
        sigemptyset(&act.sa_mask);
    }

    SignalHandler &SignalHandler::getInstance() noexcept {
        static SignalHandler signalHandler;
        return signalHandler;
    }

    std::size_t SignalHandler::getSignalSubscribersCount(int signal) noexcept {
        const auto subs = callbacks.find(signal);
        return (subs == callbacks.cend() ? 0 : subs->second.size());
    }

    void SignalHandler::signalHandler(int signum, siginfo_t *info, void *ucontext) noexcept {
        for (const auto &it: SignalHandler::getInstance().callbacks[signum]) {
            try {
                it(signum, info, ucontext);
            } catch (const std::exception &e) {
                std::cerr << "Caught exception on signal callback: " << e.what() << '\n';
            } catch (...) {
                std::cerr << "Caught unknown exception on signal callback\n";
            }
        }
    }

    void SignalHandler::insertSignal(const OnSignalCallback &cb, int signum) {
        if (auto it = callbacks.find(signum); it != callbacks.end()) {
            it->second.push_back(cb);
            return;
        }

        callbacks.try_emplace(signum).first->second.push_back(cb);
    }
}