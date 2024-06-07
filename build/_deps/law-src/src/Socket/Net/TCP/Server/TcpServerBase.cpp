#include "law/Socket/Net/TCP/Client/TcpClientBase.h"
#include "law/Socket/Net/TCP/Server/TcpServerBase.h"

namespace law::socket::net::tcp::detail {
    TcpServerBase::TcpServerBase(InternetAddress &&addr) : address(std::move(addr)) {
        openSocket();
    }

    TcpServerBase::TcpServerBase(const InternetAddress &addr) : address(addr) {
        openSocket();
    }

    Status TcpServerBase::start(std::uint16_t maxConnectionInQueue) const noexcept {
        BOOST_LEAF_CHECK(reuseAddress());
        BOOST_LEAF_CHECK(reusePort());

        const auto addr = address.getNativeAddress();
        if (bind(getNativeHandler(), reinterpret_cast<const sockaddr *>(&addr), sizeof(addr)) == -1) {
            return fromErrno();
        }

        if (listen(getNativeHandler(), maxConnectionInQueue) == -1) {
            return fromErrno();
        }

        return {};
    }

    [[nodiscard]] SocketFd TcpServerBase::acceptConnection(const AcceptOption &ao) const noexcept {
        int fd = ::accept4(getNativeHandler(), nullptr, nullptr, ao.getAsIntegral());
        if (fd == -1) {
            return fromErrno();
        }
        return fd;
    }
}