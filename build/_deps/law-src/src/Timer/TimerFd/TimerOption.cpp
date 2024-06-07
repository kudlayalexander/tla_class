#include <sys/timerfd.h>

#include "law/Timer/TimerFd/TimerOption.h"

namespace law::timer::tfd {
    TimerOption TimerOption::operator&(const TimerOption &co) const noexcept {
        return {getAsIntegral() & co.getAsIntegral()};
    }

    TimerOption TimerOption::operator|(const TimerOption &co) const noexcept {
        return {getAsIntegral() | co.getAsIntegral()};
    }

    AbsTime::AbsTime() noexcept: TimerOption(TFD_TIMER_ABSTIME) {}
}