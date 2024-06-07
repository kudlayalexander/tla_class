#include "law/Socket/CAN/J1939/J1939Address.h"

namespace law::socket::can {
    J1939Address::J1939Address(
            std::string_view ifName, std::uint8_t address,
            std::uint32_t pgn, std::uint64_t name) : CanDevice(ifName) {
            auto &j1939Addr = getNativeAddress().can_addr.j1939;
            j1939Addr.name = name;
            j1939Addr.pgn = pgn;
            j1939Addr.addr = address;
    }
}