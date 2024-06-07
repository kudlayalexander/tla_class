#include <algorithm>
#include <string_view>

#include <boost/assert.hpp>

#include "law/Signal/Signal.h"
#include "law/RetCode/RetCode.h"
#include "law/Timer/SignalTimer.h"

namespace law::timer {
    void timerSignalHandler(int, siginfo_t *info, void *) {
        reinterpret_cast<SignalTimer *>(info->si_value.sival_ptr)->onTimerHandler();
    }

    SignalTimer::~SignalTimer() noexcept {
        if (timerid) {
            timer_delete(timerid);
        }
    }

    bool SignalTimer::isArmed() const noexcept {
        itimerspec currentValue{};
        if (timer_gettime(timerid, &currentValue) == -1) {
            return false;
        }

        const auto timerValueAsByteArray = std::string_view(
                reinterpret_cast<const char *>(&currentValue),
                sizeof(itimerspec)
        );
        return !(std::ranges::all_of(timerValueAsByteArray, [](auto byte) { return (byte == 0); }));
    }

    std::chrono::nanoseconds SignalTimer::getPeriod() const noexcept {
        using namespace std::chrono;
        auto secInNs = duration_cast<nanoseconds>(seconds(its.it_value.tv_sec));
        return (secInNs + nanoseconds(its.it_value.tv_nsec));
    }

    void SignalTimer::setOnTimerHandler(OnTimer &&cb) noexcept {
        BOOST_ASSERT_MSG(cb, "OnTimer callback can't be null");
        onTimerHandler = std::move(cb);
    }

    Status SignalTimer::setPeriod(std::chrono::nanoseconds period) noexcept {
        return boost::leaf::try_handle_some(
                [this, period]() -> ErrorCode::Status {
                    using namespace std::chrono;
                    signal::SignalHandler::getInstance().subscribe(timerSignalHandler, SIGRTMIN);

                    sigevent sev{};
                    sev.sigev_notify = SIGEV_SIGNAL;
                    sev.sigev_signo = SIGRTMIN;
                    sev.sigev_value.sival_ptr = this;

                    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1) {
                        return fromErrno();
                    }

                    time_t sec;
                    long nsec;

                    if (period < 1s) {
                        sec = 0;
                        nsec = long(period.count());
                    } else {
                        auto chronoSec = duration_cast<seconds>(period);
                        sec = time_t(chronoSec.count());
                        nsec = long(
                                (duration_cast<nanoseconds>(chronoSec) - duration_cast<nanoseconds>(period)).count());
                    }

                    its = {.it_value = {.tv_sec = sec, .tv_nsec = nsec}};
                    if (timerMode == TimerMode::REPEAT) {
                        its.it_interval = {.tv_sec = sec, .tv_nsec = nsec};
                    }

                    return {};
                }
        );
    }

    void SignalTimer::setMode(TimerMode tm) noexcept {
        timerMode = tm;
    }

    Status SignalTimer::start() noexcept {
        BOOST_ASSERT_MSG(timerid, "timer not init");
        if (timer_settime(timerid, 0, &its, nullptr) == -1) {
            return fromErrno();
        }
        return {};
    }

    Status SignalTimer::stop() noexcept {
        itimerspec stopIts{
                .it_interval = {.tv_sec = 0, .tv_nsec = 0},
                .it_value = {.tv_sec = 0, .tv_nsec = 0}
        };

        if (timer_settime(timerid, 0, &stopIts, nullptr) == -1) {
            return fromErrno();
        }

        return {};
    }
}