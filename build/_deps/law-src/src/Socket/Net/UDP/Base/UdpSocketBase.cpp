#include "law/Socket/Net/UDP/Base/UdpSocketBase.h"

namespace law::socket::net::udp::detail {
    void UdpSocketBase::openSocket() {
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

    Status UdpSocketBase::makeSocket() noexcept {
        return Base::makeSocket(socket::detail::SocketType::DGRAM);
    }
}