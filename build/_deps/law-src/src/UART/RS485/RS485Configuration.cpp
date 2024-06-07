#include "law/UART/RS485/RS485Configuration.h"

namespace law::uart::rs485 {
    RS485Configuration::RS485Configuration() noexcept {
        std::memset(&serialPortConfiguration, 0, sizeof(serialPortConfiguration));
        serialPortConfiguration.flags |= SER_RS485_ENABLED;
    }

    void RS485Configuration::setDelayRtsBeforeSend(const std::chrono::microseconds &delay) noexcept {
        serialPortConfiguration.delay_rts_before_send = delay.count();
    }

    void RS485Configuration::setDelayRtsAfterSend(const std::chrono::microseconds &delay) noexcept {
        serialPortConfiguration.delay_rts_after_send = delay.count();
    }

    const serial_rs485 &RS485Configuration::getConfiguration() const noexcept {
        return serialPortConfiguration;
    }
}