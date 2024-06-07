#pragma once

#include <sys/un.h>
#include <string_view>
#include <sys/socket.h>

namespace law::socket::uds {
    class UnixAddress {
    private:
        sockaddr_un unixAddress{
            .sun_family = AF_UNIX
        };

    public:
        UnixAddress() noexcept = default;

        UnixAddress(std::string_view path) noexcept;

        void setPath(std::string_view path) noexcept;

        [[nodiscard]] std::string_view getPath() const noexcept;

        [[nodiscard]] const sockaddr_un &getNativeAddress() const noexcept;

        void setNativeAddress(const sockaddr_un &addr) noexcept;

        bool operator==(const UnixAddress &other) const noexcept;

        bool operator<(const UnixAddress &other) const noexcept;
    };
}