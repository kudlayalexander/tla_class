#pragma once

#include <etl/string.h>

#include <sys/un.h>

#include "law/Utils.h"
#include "detail/IpResolver.h"

namespace law::socket::net {
    class InternetAddress {
    private:
        sockaddr_in nativeInternetAddress{};

    public:
        InternetAddress() noexcept = default;

        InternetAddress(std::uint16_t p) noexcept;

        InternetAddress(std::string_view ip, std::uint16_t port);

        InternetAddress(const InternetAddress &copy) noexcept = default;

        InternetAddress(InternetAddress &&other) noexcept;

        InternetAddress &operator=(const InternetAddress &rhs);

        InternetAddress &operator=(InternetAddress &&rhs) noexcept;

        void set(std::uint16_t p) noexcept;

        Status set(std::string_view address, std::uint16_t p) noexcept;

        [[nodiscard]] std::string_view getInternetAddress() const noexcept;

        [[nodiscard]] std::uint16_t getPort() const noexcept;

        [[nodiscard]] sockaddr_in getNativeAddress() const noexcept;

    private:
        Status applyAddress(std::string_view hostname, std::uint16_t port) noexcept;

        [[nodiscard]] sockaddr_in makeInternetAddress(in_addr addr, std::uint16_t port) const noexcept;
    };
}
