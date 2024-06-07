#include "law/RetCode/RetCode.h"
#include "law/Socket/CAN/CanDevice.h"

namespace law::socket::can {
    CanDevice::CanDevice(std::string_view ifName) {
        if (!setInterfaceName(ifName)) {
            throw std::system_error(errno, std::system_category());
        }
    }

    Status CanDevice::setInterfaceName(std::string_view ifName) noexcept {
        name.assign(ifName.data(), ifName.size());
        BOOST_LEAF_AUTO(addr, makeCanAddr());
        nativeAddr = addr;
        return {};
    }

    std::string_view CanDevice::getInterfaceName() const noexcept {
        return {name.data(), name.size()};
    }

    const sockaddr_can &CanDevice::getNativeAddress() const noexcept {
        return nativeAddr;
    }

    [[nodiscard]] sockaddr_can &CanDevice::getNativeAddress() noexcept {
        return nativeAddr;
    }

    CanDevice::CanAddrRet CanDevice::makeCanAddr() const noexcept {
        BOOST_LEAF_AUTO(ifReq, devNameToIfReq());
        sockaddr_can addr = {
                .can_family = AF_CAN,
                .can_ifindex = ifReq.ifr_ifindex,
        };
        return addr;
    }

    CanDevice::IfreqRet CanDevice::devNameToIfReq() const noexcept {
        ifreq ifr{};
        bzero(&ifr, sizeof(ifr));
        std::strncpy(ifr.ifr_name, name.c_str(), IFNAMSIZ - 1);
        ifr.ifr_ifindex = static_cast<int>(if_nametoindex(ifr.ifr_name));
        if (!ifr.ifr_ifindex) {
            return fromErrno();
        }
        return ifr;
    }
}