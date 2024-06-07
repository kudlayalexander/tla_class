#include "law/Utils.h"
#include "law/RetCode/RetCode.h"
#include "law/Socket/Unix/UDP/Client/UdsUdpClientBase.h"

namespace law::socket::uds::udp::detail {
    UdsUdpClientBase::UdsUdpClientBase() {
        openSocket();
    }

    UdsUdpClientBase::UdsUdpClientBase(const UnixAddress &srvAddr) : serverAddress(srvAddr) {
        openSocket();
    }

    UdsUdpClientBase::~UdsUdpClientBase() noexcept {
        if (operator bool()) {
            unlink(selfAddress.getPath().data());
        }
    }

    [[nodiscard]] const UnixAddress &UdsUdpClientBase::getServerAddress() const noexcept {
        return serverAddress;
    }

    void UdsUdpClientBase::setServerAddress(const UnixAddress &addr) noexcept {
        serverAddress = addr;
    }

    Status UdsUdpClientBase::send(std::string_view data) const noexcept {
        return detail::UdsUdpSocketBase::send(data, serverAddress);
    }

    ReadCountRet UdsUdpClientBase::read(void *extBuffer, std::size_t size, const ReadOption &ro) const noexcept {
        const auto recBytes = recv(getNativeHandler(), extBuffer, size, ro.getAsIntegral());

        if (recBytes == -1) {
            return fromErrno();
        }

        if (recBytes == 0) {
            return boost::leaf::new_error(RetCode::READ_EMPTY_DATA);
        }

        return recBytes;
    }

    void UdsUdpClientBase::openSocket() {
        return boost::leaf::try_handle_all(
                [this]() -> boost::leaf::result<void> {
                    BOOST_LEAF_CHECK(makeSocket());
                    const auto path = makeRandomUnixPath();
                    selfAddress.setPath({path.data(), path.size()});
                    return
                            (bind(getNativeHandler(),
                                  reinterpret_cast<const sockaddr *>(&selfAddress.getNativeAddress()),
                                  sizeof(sockaddr_un)) == -1) ?
                            fromErrno() :
                            boost::leaf::result<void>();
                },
                [](RetCode e) {
                    throw std::system_error(std::error_code(static_cast<int>(e), std::generic_category()));
                },
                [](const std::exception &e) {
                    throw e;
                },
                []() {
                    throw std::runtime_error("Unknown error");
                }
        );
    }
}