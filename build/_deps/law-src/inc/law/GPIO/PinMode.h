#pragma once

#include <cstdint>

namespace law::gpio {
    enum class PinMode : std::uint8_t {
        INPUT = 0,
        OUTPUT = 1
    };
}