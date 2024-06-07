#pragma once

#include <boost/assert.hpp>

#include "Edge.h"
#include "Port.h"
#include "PinMode.h"
#include "law/FileDescriptor/FileDescriptor.h"

namespace law::gpio {
    class SysfsGPIO : public fd::FileDescriptor {
    private:
        static constexpr std::uint8_t kQuantityOfPinsInPort = 32;
        static constexpr char kCommonPinPath[] = "/sys/class/gpio/gpio";

    private:
        using Base = fd::FileDescriptor;

        using Base::applyOption;
        using Base::readOption;

    private:
        std::uint16_t sysfsPinNum = 0xFFFF;

    public:
        SysfsGPIO() noexcept = default;

        SysfsGPIO(Port gpioPort, std::uint8_t pinNumber);

        Status setPinMode(PinMode mode) noexcept;

        Status setPin(Port port, std::uint8_t pinNumber) noexcept;

        Status setEdge(Edge edge) noexcept;

        Status set() noexcept;

        Status reset() noexcept;

        Status setState(bool status) noexcept;

        BoolRet get() noexcept;

    private:
        static std::uint16_t gpioPinPortToSysfsPinNumber(Port port, std::uint8_t pinNumber) noexcept;

        static bool isFileExported(std::string_view filePath) noexcept;

        static Status exportPin(std::uint16_t sysfsPinNumber) noexcept;

        Status resetValueFilePos() noexcept;
    };
}