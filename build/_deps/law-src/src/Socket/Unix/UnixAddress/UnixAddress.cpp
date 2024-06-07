#include <cstring>

#include <Utils/Hash/CRC32/CRC32.h>

#include "law/Socket/Unix/UnixAddress/UnixAddress.h"

namespace law::socket::uds {
    UnixAddress::UnixAddress(std::string_view path) noexcept {
        std::strncpy(unixAddress.sun_path, path.data(), sizeof(unixAddress.sun_path));
    }

    void UnixAddress::setPath(std::string_view path) noexcept {
        std::strncpy(unixAddress.sun_path, path.data(), sizeof(unixAddress.sun_path));
    }

    [[nodiscard]] std::string_view UnixAddress::getPath() const noexcept {
        return unixAddress.sun_path;
    }

    [[nodiscard]] const sockaddr_un &UnixAddress::getNativeAddress() const noexcept {
        return unixAddress;
    }

    void UnixAddress::setNativeAddress(const sockaddr_un &addr) noexcept {
        unixAddress.sun_family = addr.sun_family;
        std::strncpy(unixAddress.sun_path, addr.sun_path, sizeof(unixAddress.sun_path));
    }

    bool UnixAddress::operator==(const UnixAddress &other) const noexcept {
        return (std::strcmp(unixAddress.sun_path, other.unixAddress.sun_path) == 0);
    }

    bool UnixAddress::operator<(const UnixAddress &other) const noexcept {
        return Utils::CRC32(other.getPath()).get() < Utils::CRC32(unixAddress.sun_path).get();
    }
}