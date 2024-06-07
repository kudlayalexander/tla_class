#include <fcntl.h>

#include "law/Socket/Base/SocketBase.h"

namespace law::socket::detail {
    Status SocketBase::nonblock(bool state) const noexcept {
        auto flags = fcntl(getNativeHandler(), F_GETFL, 0);
        if (flags == -1) {
            return fromErrno();
        }

        if (state & (flags & O_NONBLOCK)) {
            return {};
        }

        if (!state & !(flags & O_NONBLOCK)) {
            return {};
        }

        flags = state ? (flags | O_NONBLOCK) : (flags & (~O_NONBLOCK));
        return (fcntl(getNativeHandler(), F_SETFL, flags) == -1) ? fromErrno() : Status();
    }

    [[nodiscard]] BoolRet SocketBase::nonblock() const noexcept {
        const auto flags = fcntl(getNativeHandler(), F_GETFL, 0);
        if (flags == -1) {
            return fromErrno();
        }
        return (flags & O_NONBLOCK);
    }

    Status SocketBase::setReadTimeout(std::chrono::microseconds timeout) noexcept {
        return setTimeout(timeout, SocketOption::RCVTIMEO);
    }

    Status SocketBase::setWriteTimeout(std::chrono::microseconds timeout) noexcept {
        return setTimeout(timeout, SocketOption::SNDTIMEO);
    }

    Status SocketBase::resetReadTimeout() noexcept {
        return setTimeout(std::chrono::microseconds(0), SocketOption::RCVTIMEO);
    }

    Status SocketBase::resetWriteTimeout() noexcept {
        return setTimeout(std::chrono::microseconds(0), SocketOption::SNDTIMEO);
    }

    void SocketBase::setDefaultReadCount(std::size_t rc) noexcept {
        currentDefaultReadCount = rc;
    }

    std::size_t SocketBase::getDefaultReadCount() const noexcept {
        return currentDefaultReadCount;
    }

    Status SocketBase::makeSocket(SocketDomain socketDomain, SocketType socketType, int protocol) noexcept {
        return boost::leaf::try_handle_some(
                [this, socketDomain, socketType, protocol]() -> Status {
                    if (!operator bool()) {
                        auto fd = ::socket(int(socketDomain), int(socketType), protocol);
                        setNativeHandler(&fd);
                        return ((operator bool()) ? Status() : fromErrno());
                    } else {
                        return {};
                    }
                }
        );
    }

    Status SocketBase::setTimeout(std::chrono::microseconds timeout, SocketOption type) const noexcept {
        using namespace std::chrono;

        timeval tv{};
        if (timeout < 1s) {
            tv.tv_sec = 0;
            tv.tv_usec = long(timeout.count());
        } else {
            auto chronoSec = duration_cast<seconds>(timeout);
            tv.tv_sec = time_t(chronoSec.count());
            tv.tv_usec = long((duration_cast<microseconds>(chronoSec) - duration_cast<microseconds>(timeout)).count());
        }

        return applyOption(SocketOptionLevel::SOCKET, type, &tv);
    }
}