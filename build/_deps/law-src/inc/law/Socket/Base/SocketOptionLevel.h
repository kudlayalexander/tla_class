#pragma once

#include <linux/can.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/j1939.h>
#include <linux/can/isotp.h>

namespace law::socket {
    enum class SocketOptionLevel {
        SOCKET = SOL_SOCKET,
        IP = IPPROTO_IP,
        ICMP = IPPROTO_ICMP,
        IGMP = IPPROTO_IGMP,
        IPIP = IPPROTO_IPIP,
        TCP = IPPROTO_TCP,
        EGP = IPPROTO_EGP,
        PUP = IPPROTO_PUP,
        UDP = IPPROTO_UDP,
        IDP = IPPROTO_IDP,
        TP = IPPROTO_TP,
        DCCP = IPPROTO_DCCP,
        IPV6 = IPPROTO_IPV6,
        RSVP = IPPROTO_RSVP,
        GRE = IPPROTO_GRE,
        ESP = IPPROTO_ESP,
        AH = IPPROTO_AH,
        MTP = IPPROTO_MTP,
        BETTTPH = IPPROTO_BEETPH,
        ENCAP = IPPROTO_ENCAP,
        PIM = IPPROTO_PIM,
        COMP = IPPROTO_COMP,
        SCTP = IPPROTO_SCTP,
        UDPLITE = IPPROTO_UDPLITE,
        MPLS = IPPROTO_MPLS,
        RAW = IPPROTO_RAW,
        HOPOPTS_IPV6 = IPPROTO_HOPOPTS,
        ROUTING_IPV6 = IPPROTO_ROUTING,
        FRAGMENT_IPV6 = IPPROTO_FRAGMENT,
        ICMPV6_IPV6 = IPPROTO_ICMPV6,
        NONE_IPV6 = IPPROTO_NONE,
        DSTOPTS_IPV6 = IPPROTO_DSTOPTS,
        MH_IPV6 = IPPROTO_MH,
        RAW_CAN = SOL_CAN_RAW,
        J1939 = SOL_CAN_J1939,
        ISOTP = SOL_CAN_ISOTP
    };
}