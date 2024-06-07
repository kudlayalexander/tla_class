#pragma once

#include "law/Types.h"
#include "law/UART/Base/UartBase.h"
#include "law/UART/UartConfiguration.h"
#include "law/FileDescriptor/FileDescriptor.h"
#include "law/UART/RS485/RS485Configuration.h"

namespace law::uart::rs485::detail {
    class RS485Base : public law::uart::detail::UartBase {
    private:
        static constexpr std::uint8_t kDefaultReadCount = UINT8_MAX;

    private:
        using UartConfiguration = law::uart::UartConfiguration;
        using Base = law::uart::detail::UartBase;
        using RS485Configuration = law::uart::rs485::RS485Configuration;

        using Base::attach;
        using Base::reattach;
        using Base::UartBase;

    public:
        RS485Base() noexcept = default;

        explicit RS485Base(std::string_view tty, const UartConfiguration &uartConf, const RS485Configuration &rs485Conf);

        ~RS485Base() noexcept override;

        Status reattach(std::string_view tty, const UartConfiguration &uartConf, const RS485Configuration &rs485Conf);

        Status attach(std::string_view tty, const UartConfiguration &uartConf, const RS485Configuration &rs485Conf);
    };
}