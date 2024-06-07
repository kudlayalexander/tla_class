#pragma once

#include "ISOTPOptions.h"
#include "ISOTPDescriptor.h"
#include "law/Socket/CAN/Base/CanBase.h"

namespace law::socket::can::isotp::detail {
    class ISOTPBase : public can::detail::CanBase {
    private:
        using Base = can::detail::CanBase;

    public:
        ISOTPBase() noexcept = default;

        ISOTPBase(const ISOTPDescriptor &desc);

        ISOTPBase(ISOTPDescriptor &&desc);

        Status init() noexcept;

        Status applyGenericOptions(const GenericOptions &opts) noexcept;

        GenericOptionsRet getGenericOptions() const noexcept;

        Status applyFlowControlFrameOptions(const FlowControlFrameOptions &opts) noexcept;

        FlowControlFrameOptionsRet getFlowControlFrameOptions() const noexcept;

        Status applyLinkLayerOptions(const LinkLayerOptions &opts) noexcept;

        LinkLayerOptionsRet getLinkLayerOptions() const noexcept;

        Status send(std::string_view data) noexcept;
    };
}