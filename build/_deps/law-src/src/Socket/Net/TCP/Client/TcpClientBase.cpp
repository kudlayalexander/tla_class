#include <sys/poll.h>

#include "law/RetCode/RetCode.h"
#include "law/Socket/Net/TCP/Client/TcpClientBase.h"

namespace law::socket::net::tcp::detail {
    TcpClientBase::TcpClientBase() {
        Base::openSocket();
    }

    TcpClientBase::TcpClientBase(InternetAddress &&addr) : serverAddress(std::move(addr)) {
        Base::openSocket();
    }

    TcpClientBase::TcpClientBase(const InternetAddress &addr) : serverAddress(addr) {
        Base::openSocket();
    }

    TcpClientBase::TcpClientBase(TcpClientBase &&other) noexcept:
            Base(std::move(other)),
            serverAddress(std::move(other.serverAddress)) {}

    TcpClientBase &TcpClientBase::operator=(TcpClientBase &&rhs) noexcept {
        if (&rhs != this) {
            serverAddress = std::move(rhs.serverAddress);
            Base::operator=(std::move(rhs));
        }
        return *this;
    }

    [[nodiscard]] const InternetAddress &TcpClientBase::getServerAddress() const noexcept {
        return serverAddress;
    }

    Status TcpClientBase::reconnect(const std::chrono::milliseconds &timeout) noexcept {
        BOOST_LEAF_CHECK(disconnect());
        BOOST_LEAF_CHECK(connect(timeout));
        return {};
    }

    Status TcpClientBase::reconnect() noexcept {
        BOOST_LEAF_CHECK(disconnect());
        BOOST_LEAF_CHECK(connect());
        return {};
    }

    Status TcpClientBase::connect(const std::chrono::milliseconds &timeout) const noexcept {
        BOOST_LEAF_AUTO(alreadyNonblock, Base::nonblock());
        return boost::leaf::try_handle_some(
                [this, &timeout, alreadyNonblock]() -> Status {
                    if (!alreadyNonblock) {
                        BOOST_LEAF_CHECK(Base::nonblock(true));
                    }

                    if (!connect()) {
                        if (errno != EINPROGRESS) {
                            if (!alreadyNonblock) {
                                BOOST_LEAF_CHECK(Base::nonblock(false));
                            }
                            return fromErrno();
                        } else {
                            BOOST_LEAF_CHECK(waitConnect(timeout.count()));
                        }
                    }

                    if (!alreadyNonblock) {
                        BOOST_LEAF_CHECK(Base::nonblock(false));
                    }

                    return {};
                },
                [this, alreadyNonblock](boost::leaf::match<RetCode, RetCode::X_ECONNREFUSED>) -> Status {
                    if (!alreadyNonblock) {
                        BOOST_LEAF_CHECK(Base::nonblock(false));
                    }
                    return boost::leaf::new_error(RetCode::X_ECONNREFUSED);
                }
        );
    }

    Status TcpClientBase::connect() const noexcept {
        const auto tmp = serverAddress.getNativeAddress();
        const auto addr = reinterpret_cast<const struct sockaddr *>(&tmp);
        return (::connect(getNativeHandler(), addr, sizeof(tmp)) == -1) ? fromErrno() : ErrorCode::Status();
    }

    Status TcpClientBase::disconnect() noexcept {
        BOOST_LEAF_CHECK(Base::close());
        BOOST_LEAF_CHECK(Base::makeSocket());
        return {};
    }

    Status TcpClientBase::send(std::string_view data, const SendOption &so) const noexcept {
        if (::send(getNativeHandler(), data.data(), data.size(), so.getAsIntegral()) == -1) {
            return fromErrno();
        }

        return {};
    }

    [[nodiscard]] ReadCountRet TcpClientBase::read(void *extBuffer, std::size_t readCount, const ReadOption &ro) const noexcept {
        const auto recBytes = recv(getNativeHandler(), extBuffer, readCount, ro.getAsIntegral());

        if (recBytes == -1) {
            return fromErrno();
        }

        if (recBytes == 0) {
            return boost::leaf::new_error(RetCode::READ_EMPTY_DATA);
        }

        return recBytes;
    }

    [[nodiscard]] SocketError TcpClientBase::getSocketError() const noexcept {
        int error{};
        BOOST_LEAF_CHECK(SocketBase::readOption(SocketOptionLevel::SOCKET, SocketOption::ERROR, &error));
        return error;
    }

    [[nodiscard]] Status TcpClientBase::waitConnect(ssize_t connectTimeoutMs) const noexcept {
        pollfd pfd{.fd = getNativeHandler(), .events = POLLOUT};

        if (const auto pollResult = poll(&pfd, 1, connectTimeoutMs);
                pollResult == -1 && errno != EINTR) {
            return fromErrno();
        } else if (pollResult > 0) {
            BOOST_LEAF_AUTO(error, getSocketError());
            if (error) {
                return boost::leaf::new_error(RetCode(error));
            } else {
                return {};
            }
        }

        return boost::leaf::new_error(RetCode::TIMEOUT);
    }
}