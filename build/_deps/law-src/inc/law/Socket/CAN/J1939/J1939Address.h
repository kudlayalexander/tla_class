#pragma once

#include <linux/can/j1939.h>

#include "law/Socket/CAN/CanDevice.h"

namespace law::socket::can {
    class J1939Address final : public CanDevice {
    public:
        J1939Address(
                std::string_view ifName,
                std::uint8_t address = J1939_NO_ADDR,
                std::uint32_t pgn = J1939_NO_PGN,
                std::uint64_t name = J1939_NO_NAME);
    };
}