#pragma once

#include <chrono>
#include <linux/serial.h>

#include "law/Types.h"
#include "law/RetCode/RetCode.h"

namespace law::uart::rs485 {
    class RS485Configuration {
    private:
        serial_rs485 serialPortConfiguration{};

    public:
        RS485Configuration() noexcept;

        void setDelayRtsBeforeSend(const std::chrono::microseconds &delay) noexcept;

        void setDelayRtsAfterSend(const std::chrono::microseconds &delay) noexcept;

        [[nodiscard]] const serial_rs485 &getConfiguration() const noexcept;
    };
}