#include "law/Socket/Unix/Base/UnixSocketBase.h"

namespace law::socket::uds::detail {
    Status UnixSocketBase::makeSocket(socket::detail::SocketType socketType) noexcept {
        return socket::detail::SocketBase::makeSocket(socket::detail::SocketDomain::UNIX, socketType);
    }
}