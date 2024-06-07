#pragma once

#include <chrono>

#include "SocketType.h"
#include "SocketDomain.h"
#include "SocketOption.h"
#include "SocketOptionLevel.h"

#include "law/Types.h"
#include "law/RetCode/RetCode.h"
#include "law/FileDescriptor/FileDescriptor.h"

namespace law::socket::detail {
    class SocketBase : public fd::FileDescriptor {
    private:
        static constexpr std::uint8_t DEFAULT_READ_COUNT = std::numeric_limits<std::uint8_t>::max();

    private:
        std::size_t currentDefaultReadCount = DEFAULT_READ_COUNT;

    public:
        Status nonblock(bool state) const noexcept;

        [[nodiscard]] BoolRet nonblock() const noexcept;

        Status applyOption(SocketOptionLevel level, SocketOption socketOption, const auto *optionValue) const noexcept {
            return applyOption(level, int(socketOption), optionValue);
        }

        Status readOption(SocketOptionLevel level, SocketOption socketOption, auto *optionValue) const noexcept {
            return readOption(level, int(socketOption), optionValue);
        }

        Status setReadTimeout(std::chrono::microseconds timeout) noexcept;

        Status setWriteTimeout(std::chrono::microseconds timeout) noexcept;

        Status resetReadTimeout() noexcept;

        Status resetWriteTimeout() noexcept;

        void setDefaultReadCount(std::size_t rc) noexcept;

        [[nodiscard]] std::size_t getDefaultReadCount() const noexcept;

    protected:
        Status makeSocket(SocketDomain socketDomain, SocketType socketType, int protocol = 0) noexcept;

        Status applyOption(SocketOptionLevel level, auto socketOption, const auto *optionValue) const noexcept {
            return (setsockopt(getNativeHandler(), int(level), int(socketOption), optionValue, sizeof(*optionValue)) == -1) ?
                   fromErrno() :
                   Status();
        }

        Status readOption(SocketOptionLevel level, auto socketOption, auto *optionValue) const noexcept {
            socklen_t valueSize = sizeof(*optionValue);
            return (getsockopt(getNativeHandler(), int(level), int(socketOption), optionValue, &valueSize) == -1) ?
                   fromErrno() :
                   Status();
        }

    private:
        Status setTimeout(std::chrono::microseconds timeout, SocketOption type) const noexcept;
    };
}
