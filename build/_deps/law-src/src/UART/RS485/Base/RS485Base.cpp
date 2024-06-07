#include <asm/ioctls.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/ioctl.h>

#include "law/RetCode/RetCode.h"
#include "law/UART/RS485/Base/RS485Base.h"
#include "law/Utils.h"

namespace law::uart::rs485::detail {
    RS485Base::RS485Base(std::string_view tty, const UartConfiguration &uartConfiguration, const RS485Configuration &rs485Configuration) {
        if (!attach(tty, uartConfiguration, rs485Configuration)) {
            throw std::system_error(errno, std::system_category());
        }
    }

    RS485Base::~RS485Base() noexcept {
        if (operator bool()) {
            flock(getNativeHandler(), LOCK_UN);
        }
    }

    Status RS485Base::reattach(std::string_view tty, const UartConfiguration &uartConfiguration, const RS485Configuration &rs485Configuration) {
        close();
        return attach(tty, uartConfiguration, rs485Configuration);
    }

    Status RS485Base::attach(std::string_view tty, const UartConfiguration &uartConfiguration, const RS485Configuration &rs485Configuration) {
        Base::attach(tty, uartConfiguration);
        BOOST_LEAF_CHECK(applyOption(TIOCSRS485, &rs485Configuration.getConfiguration()));
        return {};
    }
}