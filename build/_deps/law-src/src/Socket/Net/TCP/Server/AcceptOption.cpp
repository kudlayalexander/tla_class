#include "sys/socket.h"

#include "law/Socket/Net/TCP/Server/AcceptOption.h"

namespace law::socket::net::tcp {
    AcceptOption AcceptOption::operator|(AcceptOption ao) const noexcept {
        return {getAsIntegral() | ao.getAsIntegral()};
    }

    Nonblock::Nonblock() noexcept: AcceptOption(SOCK_NONBLOCK) {}

    CloseOnExec::CloseOnExec() noexcept: AcceptOption(SOCK_CLOEXEC) {}
}