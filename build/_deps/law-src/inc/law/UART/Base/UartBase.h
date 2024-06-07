#pragma once

#include <chrono>

#include "law/Types.h"
#include "law/UART/UartConfiguration.h"
#include "law/FileDescriptor/FileDescriptor.h"

namespace law::uart::detail {
    class UartBase : public fd::FileDescriptor {
    private:
        static constexpr std::uint8_t kDefaultReadCount = UINT8_MAX;

    private:
        using UartConfiguration = law::uart::UartConfiguration;

    private:
        UartConfiguration uartConfiguration;
        std::uint8_t currentDefaultReadCount = kDefaultReadCount;

    public:
        UartBase() noexcept = default;

        explicit UartBase(std::string_view tty, const UartConfiguration &uartConf);

        ~UartBase() noexcept override;

        Status reattach(std::string_view tty, const UartConfiguration &uartConf);

        Status attach(std::string_view tty, const UartConfiguration &uartConf);

        Status applyUartConfiguration(const UartConfiguration &uartConf) noexcept;

        [[nodiscard]] const UartConfiguration &getUartConfiguration() const noexcept;

        void setDefaultReadCount(std::uint8_t readCount) noexcept;

        [[nodiscard]] std::uint8_t getDefaultReadCount() const noexcept;

        Status send(std::string_view data) noexcept;

        ReadCountRet read(void *extBuffer, std::size_t size) const noexcept;

    private:
        Status lockTty() noexcept;
    };
}