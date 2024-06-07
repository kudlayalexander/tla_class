#pragma once

#include "J1939Types.h"
#include "J1939Address.h"

#include "law/Socket/CAN/Base/CanBase.h"

namespace law::socket::can::detail {
    class J1939Base : public CanBase {
    private:
        using Base = detail::CanBase;

        using Base::bind;
        using Base::setCanDevice;

    public:
        J1939Base() noexcept = default;

        explicit J1939Base(const J1939Address &address);

        Status reattach() noexcept;

        Status reattach(const J1939Address &address) noexcept;

        Status attach(const J1939Address &address) noexcept;

        Status attach() noexcept;

        Status detach() noexcept;

        [[nodiscard]] Status connect(const J1939Address &destination) const noexcept;

        Status applyFilter(const J1939Filter &filter) const noexcept;

        Status applyFilter(J1939FilterSequence filterSequence) const noexcept;

        Status dropAllFilters() const noexcept;

        Status setSendPriority(J1939SendPriority priority) const noexcept;

        J1939SendPriorityRet getSendPriority() const noexcept;

        Status broadcast(bool status) const noexcept;

        BoolRet broadcast() const noexcept;

        Status send(std::string_view data) const noexcept;

        Status send(std::string_view data, const J1939Address &destination) const noexcept;

        ReadCountRet read(void *extBuffer, std::size_t readCount) const noexcept;

        ReadResultRet readFrom(void *extBuffer, std::size_t readCount) const noexcept;
    };
}