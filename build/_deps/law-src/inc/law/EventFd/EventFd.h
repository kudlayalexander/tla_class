#pragma once

#include "OpenOption.h"
#include "EventFdTypes.h"
#include "law/FileDescriptor/FileDescriptor.h"

namespace law::efd {
    class EventFd final : public law::fd::FileDescriptor {
    private:
        using Base = law::fd::FileDescriptor;
        OpenOption openOptions;

    public:
        EventFd() noexcept = default;

        explicit EventFd(std::uint32_t initialValue, const OpenOption &options = OpenOption(0));

        Status init(std::uint32_t initialValue = 0, const OpenOption &options = OpenOption(0)) noexcept;

        [[nodiscard]] const OpenOption &getOpenOptions() const noexcept;

        Status write(Value value) noexcept;

        ValueRet read() noexcept;
    };
}