#include <array>

#include <fcntl.h>
#include <unistd.h>

#include <etl/string.h>
#include <etl/to_string.h>

#include "law/File/File.h"
#include "law/GPIO/SysfsGPIO.h"

namespace law::gpio {
    SysfsGPIO::SysfsGPIO(Port gpioPort, std::uint8_t pinNumber) {
        if (!setPin(gpioPort, pinNumber)) {
            throw std::system_error(errno, std::system_category());
        }
    }

    Status SysfsGPIO::setPinMode(PinMode mode) noexcept {
        using DirectionFilePath = etl::string<34>;

        DirectionFilePath directionFilePath = kCommonPinPath;
        etl::to_string(sysfsPinNum, directionFilePath, true);
        directionFilePath += "/direction";

        file::ExtBufferFile directionFile;
        BOOST_LEAF_CHECK(
                directionFile.open(
                        {directionFilePath.data(), directionFilePath.size()},
                        file::WriteOnly()));
        return ((mode == PinMode::INPUT) ? directionFile.write("in\n") : directionFile.write("out\n"));
    }

    Status SysfsGPIO::setPin(Port port, std::uint8_t pinNumber) noexcept {
        if (pinNumber > kQuantityOfPinsInPort) {
            return boost::leaf::new_error(RetCode::INVALID_ARGUMENT);
        }

        using ValueFilePath = etl::string<29>;

        ValueFilePath valueFilePath = kCommonPinPath;
        sysfsPinNum = gpioPinPortToSysfsPinNumber(port, pinNumber);
        etl::to_string(sysfsPinNum, valueFilePath, true);
        valueFilePath += "/value";

        if (!isFileExported({valueFilePath.data(), valueFilePath.size()})) {
            BOOST_LEAF_CHECK(exportPin(sysfsPinNum));
        }

        if (operator bool()) {
            close();
        }

        auto fd = open(valueFilePath.data(), O_RDWR);
        if (fd == -1) [[unlikely]] {
            return fromErrno();
        }

        setNativeHandler(&fd);
        return {};
    }

    Status SysfsGPIO::setEdge(Edge edge) noexcept {
        using EdgeFilePath = etl::string<28>;

        EdgeFilePath edgeFilePath = kCommonPinPath;
        etl::to_string(sysfsPinNum, edgeFilePath, true);
        edgeFilePath += "/edge";

        file::ExtBufferFile directionFile;
        BOOST_LEAF_CHECK(
                directionFile.open(
                        {edgeFilePath.data(), edgeFilePath.size()},
                        file::WriteOnly()));

        static constexpr std::array<std::string_view, std::uint8_t(Edge::MAX_VALUE)> kEnumStringValues = {
                "none\n", "rising\n", "falling\n", "both\n"
        };
        return (
                (edge < Edge::MAX_VALUE) ?
                directionFile.write(kEnumStringValues[std::size_t(edge)]) :
                boost::leaf::new_error(RetCode::INVALID_ARGUMENT)
        );
    }

    Status SysfsGPIO::set() noexcept {
        return setState(true);
    }

    Status SysfsGPIO::reset() noexcept {
        return setState(false);
    }

    Status SysfsGPIO::setState(bool status) noexcept {
        const char value = '0' + status;
        BOOST_LEAF_CHECK(resetValueFilePos());
        return (write(getNativeHandler(), &value, sizeof(char)) == -1) ? fromErrno() : Status();
    }

    BoolRet SysfsGPIO::get() noexcept {
        char value;
        BOOST_LEAF_CHECK(resetValueFilePos());
        if (read(getNativeHandler(), &value, sizeof(char)) == -1) [[unlikely]] {
            return fromErrno();
        }

        return (value - '0');
    }

    std::uint16_t SysfsGPIO::gpioPinPortToSysfsPinNumber(Port port, std::uint8_t pinNumber) noexcept {
        return (static_cast<std::uint8_t>(port) * kQuantityOfPinsInPort + pinNumber - kQuantityOfPinsInPort);
    }

    bool SysfsGPIO::isFileExported(std::string_view filePath) noexcept {
        return (access(filePath.data(), F_OK) == 0);
    }

    Status SysfsGPIO::exportPin(std::uint16_t sysfsPinNumber) noexcept {
        using ExportFilePath = etl::string<sizeof("/sys/class/gpio/export") - 1>;
        ExportFilePath exportFilePath = "/sys/class/gpio/export";

        using ExportPinString = etl::string<sizeof("XXX\n") - 1>;
        ExportPinString exportPinString;
        etl::to_string(sysfsPinNumber, exportPinString);
        exportPinString += '\n';

        file::ExtBufferFile exportFile;
        BOOST_LEAF_CHECK(exportFile.open(
                {exportFilePath.data(), exportFilePath.size()},
                law::file::WriteOnly()));
        return exportFile.write({exportPinString.data(), exportPinString.size()});
    }

    Status SysfsGPIO::resetValueFilePos() noexcept {
        return lseek(getNativeHandler(), 0, SEEK_SET) == -1 ? fromErrno() : Status();
    }
}