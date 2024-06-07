#pragma once

#include "law/Socket/CAN/CanDevice.h"
#include "law/Socket/Base/SocketBase.h"

namespace law::socket::can::detail {
    class CanBase : public socket::detail::SocketBase {
    private:
        using Base = socket::detail::SocketBase;

    private:
        CanDevice canDevice;

    public:
        CanBase() noexcept = default;

        explicit CanBase(const CanDevice &device);

        explicit CanBase(CanDevice &&device);

        Status bind() const noexcept;

        void setCanDevice(const CanDevice &dev) noexcept;

        void setCanDevice(CanDevice &&dev) noexcept;

        [[nodiscard]] const CanDevice &getCanDevice() const noexcept;

    protected:
        Status makeSocket(socket::detail::SocketType socketType, int protocolFamily) noexcept;
    };
}