#pragma once

#include <sys/timerfd.h>

namespace law::timer::tfd {
    enum class ClockId : int {
        UNSPECIFIED = -1,
        REALTIME = CLOCK_REALTIME,
        MONOTONIC = CLOCK_MONOTONIC,
        BOOT_TIME = CLOCK_BOOTTIME,
        REALTIME_ALARM = CLOCK_REALTIME_ALARM,
        BOOT_TIME_ALARM = CLOCK_BOOTTIME_ALARM
    };
}