#pragma once

#include <cstdint>

namespace law::gpio {
    enum class Edge : std::uint8_t {
        DISABLE,
        RISING,
        FALLING,
        BOTH,
        MAX_VALUE
    };
}