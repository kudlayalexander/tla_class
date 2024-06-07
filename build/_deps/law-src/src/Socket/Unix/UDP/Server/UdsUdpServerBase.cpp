#include "law/Utils.h"
#include "law/Socket/Unix/UDP/Server/UdsUdpServerBase.h"

namespace law::socket::uds::udp::detail {
    UdsUdpServerBase::UdsUdpServerBase() {
        openSocket();
    }

    UdsUdpServerBase::UdsUdpServerBase(const UnixAddress &srvAddr) : serverAddress(srvAddr) {
        openSocket();
    }

    const UnixAddress &UdsUdpServerBase::getAddress() const noexcept {
        return serverAddress;
    }

    UdsUdpServerBase::~UdsUdpServerBase() noexcept {
        if (getNativeHandler() != -1) {
            unlink(serverAddress.getPath().data());
        }
    }

    Status UdsUdpServerBase::bind() noexcept {
        auto ret = ::bind(getNativeHandler(),
                                reinterpret_cast<const sockaddr *>(&serverAddress.getNativeAddress()),
                                sizeof(sockaddr_un));
        if (ret == -1 && errno == EADDRINUSE) {
            unlink(serverAddress.getPath().data());
            ret = ::bind(getNativeHandler(),
                         reinterpret_cast<const sockaddr *>(&serverAddress.getNativeAddress()),
                         sizeof(sockaddr_un));
            if (ret == -1) {
                return fromErrno();
            }
        }

        return {};
    }

    ReadResultRet UdsUdpServerBase::read(void *extBuffer, std::size_t size, const ReadOption &ro) const noexcept {
        sockaddr_un clientAddress{};
        socklen_t fromLength = sizeof(sockaddr_un);
        const auto recBytes = recvfrom(getNativeHandler(), extBuffer, size, ro.getAsIntegral(),
                                       reinterpret_cast<sockaddr *>(&clientAddress),
                                       &fromLength);

        if (recBytes == -1) {
            return fromErrno();
        }

        if (recBytes == 0) {
            return boost::leaf::new_error(RetCode::NO_AVAILABLE_SPACE);
        }

        return ReadResult{clientAddress, std::size_t(recBytes)};
    }

    void UdsUdpServerBase::openSocket() {
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
}