#pragma once

#include <net/if.h>

#include <etl/string.h>

#include "CanTypes.h"

namespace law::socket::can {
    class CanDevice {
    private:
        using IfreqRet = ErrorCode::CustomError<ifreq>;
        using CanAddrRet = ErrorCode::CustomError<sockaddr_can>;

    private:
        etl::string<IFNAMSIZ - 1> name;
        sockaddr_can nativeAddr{};

    public:
        CanDevice() noexcept = default;

        CanDevice(std::string_view ifName);

        Status setInterfaceName(std::string_view ifName) noexcept;

        [[nodiscard]] std::string_view getInterfaceName() const noexcept;

        [[nodiscard]] const sockaddr_can &getNativeAddress() const noexcept;

        [[nodiscard]] sockaddr_can &getNativeAddress() noexcept;

    private:
        [[nodiscard]] CanAddrRet makeCanAddr() const noexcept;

        [[nodiscard]] IfreqRet devNameToIfReq() const noexcept;
    };
}