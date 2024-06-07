#include <algorithm>
#include <string_view>
#include <utility>

#include <sys/timerfd.h>

#include "law/RetCode/RetCode.h"
#include "law/Timer/TimerFd/TimerFd.h"

namespace law::timer::tfd {
    TimerFd::TimerFd(ClockId cid, const CreateOption &co) :
            FileDescriptor(timerfd_create(int(cid), co.getAsIntegral())) {
        if (!create(cid, co)) {
            throw std::system_error(std::error_code(errno, std::system_category()));
        }
    }

    Status TimerFd::create(ClockId cid, const CreateOption &co) noexcept {
        auto fd = timerfd_create(int(cid), co.getAsIntegral());
        if (fd == -1) {
            return fromErrno();
        }

        clockId = cid;
        createOptions = co;
        setNativeHandler(&fd);
        return {};
    }

    bool TimerFd::isArmed() const noexcept {
        itimerspec currentValue{};
        if (timerfd_gettime(getNativeHandler(), &currentValue) == -1) {
            return false;
        }

        const auto timerValueAsByteArray = std::string_view(
                reinterpret_cast<const char *>(&currentValue),
                sizeof(itimerspec)
        );
        return !(std::ranges::all_of(timerValueAsByteArray, [](auto byte) { return (byte == 0); }));
    }

    ClockId TimerFd::getClockId() const noexcept {
        return clockId;
    }

    CreateOption TimerFd::getCreateOptions() const noexcept {
        return createOptions;
    }

    TimerOption TimerFd::getTimerOptions() const noexcept {
        return timerOptions;
    }

    std::chrono::nanoseconds TimerFd::getPeriod() const noexcept {
        return period;
    }

    void TimerFd::setTimerOptions(TimerOption to) noexcept {
        timerOptions = std::move(to);
    }

    void TimerFd::setOnTimerHandler(OnTimer &&cb) noexcept {
        onTimerHandler = std::move(cb);
    }

    Status TimerFd::setPeriod(std::chrono::nanoseconds p) noexcept {
        period = p;
        return {};
    }

    void TimerFd::setMode(TimerMode tm) noexcept {
        timerMode = tm;
    }

    Status TimerFd::start() noexcept {
        using namespace std::chrono;

        time_t sec;
        long nsec;

        if (period < 1s) {
            sec = 0;
            nsec = long(period.count());
        } else {
            auto chronoSec = duration_cast<seconds>(period);
            sec = time_t(chronoSec.count());
            nsec = long((duration_cast<nanoseconds>(period) - duration_cast<nanoseconds>(chronoSec)).count());
        }

        itimerspec timerSpec = {.it_value = {.tv_sec = sec, .tv_nsec = nsec}};
        if (timerMode == TimerMode::REPEAT) {
            timerSpec.it_interval = {.tv_sec = sec, .tv_nsec = nsec};
        }

        if (timerfd_settime(getNativeHandler(), timerOptions.getAsIntegral(), &timerSpec, nullptr) == -1) {
            return fromErrno();
        }

        return {};
    }

    Status TimerFd::stop() noexcept {
        itimerspec timerSpec{
                .it_interval = {.tv_sec = 0, .tv_nsec = 0},
                .it_value = {.tv_sec = 0, .tv_nsec = 0}
        };

        if (timerfd_settime(getNativeHandler(), 0, &timerSpec, nullptr) == -1) {
            return fromErrno();
        }

        return {};
    }

    Status TimerFd::poll() noexcept {
        std::uint64_t expiredTimes = 0;
        if (::read(getNativeHandler(), &expiredTimes, sizeof(std::uint64_t)) == -1) {
            return fromErrno();
        }

        for (std::uint64_t i = 0; i < expiredTimes; i++) {
            onTimerHandler();
        }

        return {};
    }
}
