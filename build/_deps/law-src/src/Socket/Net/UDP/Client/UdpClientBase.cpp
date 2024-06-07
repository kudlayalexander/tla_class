#include "law/Utils.h"
#include "law/Socket/Net/UDP/Client/UdpClientBase.h"

namespace law::socket::net::udp::detail {
    UdpClientBase::UdpClientBase() {
        Base::openSocket();
    }

    UdpClientBase::UdpClientBase(const InternetAddress &srvAddr) : serverAddress(srvAddr) {
        Base::openSocket();
    }

    void UdpClientBase::setServerAddress(const InternetAddress &addr) noexcept {
        serverAddress = addr;
    }

    [[nodiscard]] const InternetAddress &UdpClientBase::getServerAddress() const noexcept {
        return serverAddress;
    }

    Status UdpClientBase::send(std::string_view data, const SendOption &so) const noexcept {
        const auto addr = serverAddress.getNativeAddress();
        return LAW_INTERNAL_ERROR_CHECK(
                       sendto(
                               getNativeHandler(),
                               data.data(),
                               data.size(),
                               so.getAsIntegral(),
                               reinterpret_cast<const sockaddr *>(&addr),
                               sizeof(addr)));
    }

    ReadCountRet
    UdpClientBase::read(void *extBuffer, std::size_t size, const ReadOption &ro) const noexcept {
        const auto recBytes =
                recvfrom(
                        getNativeHandler(),
                        extBuffer,
                        size,
                        ro.getAsIntegral(),
                        nullptr,
                        nullptr);

        if (recBytes == -1) {
            return fromErrno();
        }

        if (recBytes == 0) {
            return boost::leaf::new_error(RetCode::READ_EMPTY_DATA);
        }

        return recBytes;
    }
}