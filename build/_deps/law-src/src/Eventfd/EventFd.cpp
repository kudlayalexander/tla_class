#include "law/RetCode/RetCode.h"
#include "law/EventFd/EventFd.h"

namespace law::efd {
    EventFd::EventFd(std::uint32_t initialValue, const OpenOption &options) :
            Base(eventfd(initialValue, options.getAsIntegral())),
            openOptions(options) {
        if (!operator bool()) {
            throw std::system_error(errno, std::system_category());
        }
    }

    Status EventFd::init(std::uint32_t initialValue, const OpenOption &options) noexcept {
        auto fd = eventfd(initialValue, options.getAsIntegral());
        if (fd == -1) {
            return fromErrno();
        }

        setNativeHandler(&fd);
        openOptions = options;

        return {};
    }

    [[nodiscard]] const OpenOption &EventFd::getOpenOptions() const noexcept {
        return openOptions;
    }

    Status EventFd::write(Value value) noexcept {
        return ((eventfd_write(getNativeHandler(), value) == -1) ? fromErrno() : Status());
    }

    ValueRet EventFd::read() noexcept {
        Value value;
        if (eventfd_read(getNativeHandler(), &value) == -1) {
            return fromErrno();
        }
        return value;
    }
}