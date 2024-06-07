#include <sys/socket.h>

#include "law/Socket/ReadSendOption.h"

namespace law::socket {
    ReadSendOption ReadSendOption::operator|(const ReadSendOption &rwo) const noexcept {
        return {getAsIntegral() | rwo.getAsIntegral()};
    }

    DontWait::DontWait() noexcept: ReadSendOption(MSG_DONTWAIT) {}

    OutOfBand::OutOfBand() noexcept: ReadSendOption(MSG_OOB) {}
}