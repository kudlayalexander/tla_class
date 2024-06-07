#include "law/RetCode/RetCode.h"
#include "law/Socket/Net/TCP/Base/TcpSocketBase.h"

namespace law::socket::net::tcp::detail {
    void TcpSocketBase::openSocket() {
        return boost::leaf::try_handle_all(
                [this]() -> boost::leaf::result<void> {
                    BOOST_LEAF_CHECK(makeSocket());
                    return {};
                },
                [](RetCode e) {
                    throw std::system_error(std::error_code(static_cast<int>(e), std::generic_category()));
                },
                []() {
                    throw std::runtime_error("Unknown error");
                }
        );
    }

    Status TcpSocketBase::makeSocket() noexcept {
        return Base::makeSocket(socket::detail::SocketType::STREAM);
    }

    Status TcpSocketBase::setupKeepalive(
            std::chrono::seconds startThreshold,
            std::uint32_t repeats,
            std::chrono::seconds delayBetweenRepeats) const noexcept {
        static constexpr int enable = 1;
        const auto trafficMaxDelaySec = startThreshold.count();
        const auto keepaliveRetryGapSec = delayBetweenRepeats.count();
        const auto connectionTimeout = trafficMaxDelaySec + (repeats * keepaliveRetryGapSec);

        BOOST_LEAF_CHECK(Base::applyOption(SocketOptionLevel::SOCKET, SocketOption::KEEPALIVE, &enable));
        BOOST_LEAF_CHECK(applyOption(SocketOptionLevel::TCP, TcpOption::KEEPIDLE, &trafficMaxDelaySec));
        BOOST_LEAF_CHECK(applyOption(SocketOptionLevel::TCP, TcpOption::KEEPCNT, &repeats));
        BOOST_LEAF_CHECK(applyOption(SocketOptionLevel::TCP, TcpOption::KEEPINTVL, &keepaliveRetryGapSec));
        BOOST_LEAF_CHECK(applyOption(SocketOptionLevel::TCP, TcpOption::USER_TIMEOUT, &connectionTimeout));

        return {};
    }

    Status TcpSocketBase::disableNagleAlgorithm() const noexcept {
        static constexpr int enable = 1;
        return applyOption(SocketOptionLevel::TCP, TcpOption::NODELAY, &enable);
    }

    Status TcpSocketBase::reuseAddress() const noexcept {
        static constexpr int enable = 1;
        return Base::applyOption(SocketOptionLevel::SOCKET, SocketOption::REUSEADDR, &enable);
    }

    Status TcpSocketBase::reusePort() const noexcept {
        static constexpr int enable = 1;
        return Base::applyOption(SocketOptionLevel::SOCKET, SocketOption::REUSEPORT, &enable);
    }
}