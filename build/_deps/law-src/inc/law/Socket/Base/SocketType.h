#pragma once

#include <sys/socket.h>

namespace law::socket::detail {
    enum class SocketType {
        STREAM = SOCK_STREAM,
        DGRAM = SOCK_DGRAM,
        RAW = SOCK_RAW,
        RDM = SOCK_RDM,
        SEQPACKET = SOCK_SEQPACKET,
        DCCP = SOCK_DCCP,
        PACKET = SOCK_PACKET
    };
}