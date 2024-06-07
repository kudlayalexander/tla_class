#pragma once

#include <cstdint>

namespace law::gpio {
    enum class Port : std::uint8_t {
        GPIO1 = 1,
        GPIO2 = 2,
        GPIO3 = 3,
        GPIO4 = 4,
        GPIO5 = 5,
        GPIO6 = 6,
        GPIO7 = 7
    };
}