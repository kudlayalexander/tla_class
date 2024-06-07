#pragma once

#include <cstdint>

namespace law::timer {
    enum class TimerMode : std::uint8_t {
        REPEAT,
        ONESHOT
    };
}