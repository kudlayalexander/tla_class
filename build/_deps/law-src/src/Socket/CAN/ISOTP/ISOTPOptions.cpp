#include "law/Socket/CAN/ISOTP/ISOTPOptions.h"

namespace law::socket::can::isotp {
    ISOTPFlag ISOTPFlag::operator&(const ISOTPFlag &f) const noexcept {
        return {getAsIntegral() & f.getAsIntegral()};
    }

    ISOTPFlag ISOTPFlag::operator|(const ISOTPFlag &f) const noexcept {
        return {getAsIntegral() | f.getAsIntegral()};
    }

    ListenOnlyMode::ListenOnlyMode() noexcept: ISOTPFlag(CAN_ISOTP_LISTEN_MODE) {}

    ExtendedAddress::ExtendedAddress() noexcept: ISOTPFlag(CAN_ISOTP_EXTEND_ADDR) {}

    TxPadding::TxPadding() noexcept: ISOTPFlag(CAN_ISOTP_TX_PADDING) {}

    RxPadding::RxPadding() noexcept: ISOTPFlag(CAN_ISOTP_RX_PADDING) {}

    CheckLengthPadding::CheckLengthPadding() noexcept: ISOTPFlag(CAN_ISOTP_CHK_PAD_LEN) {}

    CheckDataPadding::CheckDataPadding() noexcept: ISOTPFlag(CAN_ISOTP_CHK_PAD_DATA) {}

    HandleHalfDuplexErrorState::HandleHalfDuplexErrorState() noexcept: ISOTPFlag(CAN_ISOTP_HALF_DUPLEX) {}

    IgnoreSeparationTimeFromFlowControlFrame::IgnoreSeparationTimeFromFlowControlFrame() noexcept: ISOTPFlag(CAN_ISOTP_FORCE_TXSTMIN) {}

    IgnoreConsecutiveFrameOnRxSeparationTime::IgnoreConsecutiveFrameOnRxSeparationTime() noexcept: ISOTPFlag(CAN_ISOTP_FORCE_RXSTMIN) {}

    DifferentRxExtendedAddress::DifferentRxExtendedAddress() noexcept: ISOTPFlag(CAN_ISOTP_RX_EXT_ADDR) {}

    WaitTxCompletion::WaitTxCompletion() noexcept: ISOTPFlag(CAN_ISOTP_WAIT_TX_DONE) {}

    SingleFrameBroadcast::SingleFrameBroadcast() noexcept: ISOTPFlag(CAN_ISOTP_SF_BROADCAST) {}

    ConsecutiveFrameBroadcast::ConsecutiveFrameBroadcast() noexcept: ISOTPFlag(CAN_ISOTP_CF_BROADCAST) {}
}