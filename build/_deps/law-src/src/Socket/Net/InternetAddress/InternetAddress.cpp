#include <arpa/inet.h>

#include "law/RetCode/RetCode.h"
#include "law/Socket/Net/InternetAddress/InternetAddress.h"

namespace law::socket::net {
    InternetAddress::InternetAddress(std::uint16_t p) noexcept {
        nativeInternetAddress = makeInternetAddress({.s_addr = INADDR_ANY}, p);
    }

    InternetAddress::InternetAddress(std::string_view address, std::uint16_t port) {
        boost::leaf::try_handle_all(
                [this, address, port]() -> Status {
                    return applyAddress(address, port);
                },
                [](RetCode e) {
                    throw std::system_error(static_cast<int>(e), std::system_category());
                },
                []() {
                    throw std::runtime_error("Unknown error");
                }
        );
    }

    InternetAddress::InternetAddress(InternetAddress &&other) noexcept:
            nativeInternetAddress(other.nativeInternetAddress) {}

    InternetAddress &InternetAddress::operator=(const InternetAddress &rhs) {
        if (this != &rhs) {
            nativeInternetAddress = rhs.nativeInternetAddress;
        }
        return *this;
    }

    InternetAddress &InternetAddress::operator=(InternetAddress &&rhs) noexcept {
        if (this != &rhs) {
            nativeInternetAddress = rhs.nativeInternetAddress;
        }
        return *this;
    }

    void InternetAddress::set(std::uint16_t p) noexcept {
        nativeInternetAddress.sin_port = htons(p);
    }

    Status InternetAddress::set(std::string_view address, std::uint16_t p) noexcept {
        return applyAddress(address, p);
    }

    [[nodiscard]] std::string_view InternetAddress::getInternetAddress() const noexcept {
        return inet_ntoa(nativeInternetAddress.sin_addr);
    }

    [[nodiscard]] std::uint16_t InternetAddress::getPort() const noexcept {
        return nativeInternetAddress.sin_port;
    }

    [[nodiscard]] sockaddr_in InternetAddress::getNativeAddress() const noexcept {
        return nativeInternetAddress;
    }

    Status InternetAddress::applyAddress(std::string_view hostname, std::uint16_t port) noexcept {
        return boost::leaf::try_handle_some(
                [this, hostname, port]() -> Status {
                    BOOST_LEAF_AUTO(ipAsInAddr, detail::IpResolver::resolve(hostname));
                    nativeInternetAddress = makeInternetAddress(ipAsInAddr, port);
                    return {};
                }
        );
    }

    [[nodiscard]] sockaddr_in InternetAddress::makeInternetAddress(in_addr addr, std::uint16_t port) const noexcept {
        return {
                .sin_family = AF_INET,
                .sin_port = htons(port),
                .sin_addr = addr
        };
    }
}
