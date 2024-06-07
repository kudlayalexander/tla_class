#pragma once

#include <chrono>
#include <linux/can/isotp.h>

#include "law/Types.h"
#include "law/FlagValue.h"

namespace law::socket::can::isotp {
    struct ISOTPFlag : public UInt32FlagValue {
        using UInt32FlagValue::UInt32FlagValue;

        ISOTPFlag operator&(const ISOTPFlag &f) const noexcept;

        ISOTPFlag operator|(const ISOTPFlag &f) const noexcept;
    };

    struct ListenOnlyMode final : public ISOTPFlag {
        ListenOnlyMode() noexcept;
    };

    struct ExtendedAddress final : public ISOTPFlag {
        ExtendedAddress() noexcept;
    };

    struct TxPadding final : public ISOTPFlag {
        TxPadding() noexcept;
    };

    struct RxPadding final : public ISOTPFlag {
        RxPadding() noexcept;
    };

    struct CheckLengthPadding final : public ISOTPFlag {
        CheckLengthPadding() noexcept;
    };

    struct CheckDataPadding final : public ISOTPFlag {
        CheckDataPadding() noexcept;
    };

    struct HandleHalfDuplexErrorState final : public ISOTPFlag {
        HandleHalfDuplexErrorState() noexcept;
    };

    struct IgnoreSeparationTimeFromFlowControlFrame final : public ISOTPFlag {
        IgnoreSeparationTimeFromFlowControlFrame() noexcept;
    };

    struct IgnoreConsecutiveFrameOnRxSeparationTime final : public ISOTPFlag {
        IgnoreConsecutiveFrameOnRxSeparationTime() noexcept;
    };

    struct DifferentRxExtendedAddress final : public ISOTPFlag {
        DifferentRxExtendedAddress() noexcept;
    };

    struct WaitTxCompletion final : public ISOTPFlag {
        WaitTxCompletion() noexcept;
    };

    struct SingleFrameBroadcast final : public ISOTPFlag {
        SingleFrameBroadcast() noexcept;
    };

    struct ConsecutiveFrameBroadcast final : public ISOTPFlag {
        ConsecutiveFrameBroadcast() noexcept;
    };

    struct GenericOptions {
        ISOTPFlag flags;

        std::chrono::nanoseconds frameTxTime;

        std::uint8_t extendedAddress;
        std::uint8_t extendedAddressRx;

        std::uint8_t paddingByteTx;
        std::uint8_t paddingByteRx;
    };
    using GenericOptionsRet = ErrorCode::CustomError<GenericOptions>;

    struct FlowControlFrameOptions {
        std::uint8_t blockSize;
        std::uint8_t minSeparationTime;
        std::uint8_t frameTxWaitMaxCount;
    };
    using FlowControlFrameOptionsRet = ErrorCode::CustomError<FlowControlFrameOptions>;

    struct LinkLayerOptions {
        enum class MTUType : std::uint8_t {
            CAN = CAN_MTU,
            CANFD = CANFD_MTU
        };

        MTUType mtuType;
        std::uint8_t maxPayloadTxLength;
        std::uint8_t txFlags;
    };
    using LinkLayerOptionsRet = ErrorCode::CustomError<LinkLayerOptions>;
}