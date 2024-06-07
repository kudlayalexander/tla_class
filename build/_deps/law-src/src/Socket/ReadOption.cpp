#include <sys/socket.h>

#include "law/Socket/ReadOption.h"

namespace law::socket {
    ReadOption ReadOption::operator|(const ReadOption &ro) const noexcept {
        return {getAsIntegral() | ro.getAsIntegral()};
    }

    CmsgCloseOnExec::CmsgCloseOnExec() noexcept: ReadOption(MSG_CMSG_CLOEXEC) {}

    Peek::Peek() noexcept: ReadOption(MSG_PEEK) {}

    Truncate::Truncate() noexcept: ReadOption(MSG_TRUNC) {}

    WaitAll::WaitAll() noexcept: ReadOption(MSG_WAITALL) {}
}