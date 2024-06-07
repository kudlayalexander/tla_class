#include "law/Socket/CAN/Base/CanBase.h"

namespace law::socket::can::detail {
    CanBase::CanBase(const CanDevice &device) : canDevice(device) { }

    CanBase::CanBase(CanDevice &&device) : canDevice(std::move(device)) { }

    Status CanBase::bind() const noexcept {
        const auto addr = canDevice.getNativeAddress();
        if (::bind(getNativeHandler(), reinterpret_cast<const sockaddr *> (&addr), sizeof(addr)) < 0) {
            return fromErrno();
        }
        return {};
    }

    void CanBase::setCanDevice(const CanDevice &dev) noexcept {
        canDevice = dev;
    }

    void CanBase::setCanDevice(CanDevice &&dev) noexcept {
        canDevice = std::move(dev);
    }

    const CanDevice &CanBase::getCanDevice() const noexcept {
        return canDevice;
    }

    Status CanBase::makeSocket(socket::detail::SocketType socketType, int protocolFamily) noexcept {
        return Base::makeSocket(socket::detail::SocketDomain::CAN, socketType, protocolFamily);
    }
}