#pragma once

#include "Base/CanBase.h"

namespace law::socket::can {
    class Can final : public detail::CanBase {
    private:
        using Base = detail::CanBase;

        using Base::getDefaultReadCount;
        using Base::setDefaultReadCount;

    private:
        CanMode mode;

    public:
        Can(CanMode m = CanMode::CAN) noexcept;

        explicit Can(const CanDevice &device, CanMode m = CanMode::CAN);

        explicit Can(CanDevice &&device, CanMode m = CanMode::CAN);

        Status init() noexcept;

        Status send(CanFrameSequence frameSequence) const noexcept;

        Status send(const CanFrame &frame) const noexcept;

        [[nodiscard]] CanFrameRet read() const noexcept;

        Status applyFilter(CanFilter filter) const noexcept;

        Status applyFilter(CanFilterSequence filterSequence) const noexcept;

        Status dropAllFilters() const noexcept;

    private:
        [[nodiscard]] BoolRet isCanfdSupported() const noexcept;

        [[nodiscard]] static canfd_frame makeCanfdFrame(const CanFrame &frame) noexcept;

        [[nodiscard]] static CanFrame makeCanFrame(const canfd_frame &frame) noexcept;
    };
}
