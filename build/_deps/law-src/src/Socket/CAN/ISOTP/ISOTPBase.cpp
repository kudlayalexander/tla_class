#include "law/Utils.h"
#include "law/Socket/CAN/ISOTP/ISOTPBase.h"

namespace law::socket::can::isotp::detail {
    ISOTPBase::ISOTPBase(const ISOTPDescriptor &desc) : Base(desc) {
        if (!init()) {
            throw std::system_error(errno, std::system_category());
        }
    }

    ISOTPBase::ISOTPBase(ISOTPDescriptor &&desc) : Base(std::move(desc)) {
        if (!init()) {
            throw std::system_error(errno, std::system_category());
        }
    }

    Status ISOTPBase::init() noexcept {
        using namespace law::socket::detail;
        return makeSocket(SocketType::DGRAM, CAN_ISOTP);
    }

    Status ISOTPBase::applyGenericOptions(const GenericOptions &opts) noexcept {
        const can_isotp_options nativeOpts = {
                .flags = opts.flags.getAsIntegral(),
                .frame_txtime = std::uint8_t(opts.frameTxTime.count()),
                .ext_address = opts.extendedAddress,
                .txpad_content = opts.paddingByteTx,
                .rxpad_content = opts.paddingByteRx,
                .rx_ext_address = opts.extendedAddressRx
        };
        return applyOption(SocketOptionLevel::ISOTP, SocketOption::ISOTP_OPTS, &nativeOpts);
    }

    GenericOptionsRet ISOTPBase::getGenericOptions() const noexcept {
        can_isotp_options nativeOpts{};
        BOOST_LEAF_CHECK(readOption(SocketOptionLevel::ISOTP, SocketOption::ISOTP_OPTS, &nativeOpts));
        return GenericOptions{
                .flags = ISOTPFlag(nativeOpts.flags),
                .frameTxTime = std::chrono::nanoseconds(nativeOpts.frame_txtime),
                .extendedAddress = nativeOpts.ext_address,
                .extendedAddressRx = nativeOpts.rx_ext_address,
                .paddingByteTx = nativeOpts.txpad_content,
                .paddingByteRx = nativeOpts.rxpad_content
        };
    }

    Status ISOTPBase::applyFlowControlFrameOptions(const FlowControlFrameOptions &opts) noexcept {
        const can_isotp_fc_options nativeOpts = {
                .bs = opts.blockSize,
                .stmin = opts.minSeparationTime,
                .wftmax = opts.frameTxWaitMaxCount
        };
        return applyOption(SocketOptionLevel::ISOTP, SocketOption::ISOTP_RECV_OPTS, &nativeOpts);
    }

    FlowControlFrameOptionsRet ISOTPBase::getFlowControlFrameOptions() const noexcept {
        can_isotp_fc_options nativeOpts{};
        BOOST_LEAF_CHECK(readOption(SocketOptionLevel::ISOTP, SocketOption::ISOTP_RECV_OPTS, &nativeOpts));
        return FlowControlFrameOptions{
                .blockSize = nativeOpts.bs,
                .minSeparationTime = nativeOpts.stmin,
                .frameTxWaitMaxCount = nativeOpts.wftmax
        };
    }

    Status ISOTPBase::applyLinkLayerOptions(const LinkLayerOptions &opts) noexcept {
        const can_isotp_ll_options nativeOpts = {
                .mtu = std::uint8_t(opts.mtuType),
                .tx_dl = opts.maxPayloadTxLength,
                .tx_flags = opts.txFlags
        };
        return applyOption(SocketOptionLevel::ISOTP, SocketOption::ISOTP_LL_OPTS, &nativeOpts);
    }

    LinkLayerOptionsRet ISOTPBase::getLinkLayerOptions() const noexcept {
        can_isotp_ll_options nativeOpts{};
        BOOST_LEAF_CHECK(readOption(SocketOptionLevel::ISOTP, SocketOption::ISOTP_LL_OPTS, &nativeOpts));
        return LinkLayerOptions{
                .mtuType = LinkLayerOptions::MTUType(nativeOpts.mtu),
                .maxPayloadTxLength = nativeOpts.tx_dl,
                .txFlags = nativeOpts.tx_flags
        };
    }

    Status ISOTPBase::send(std::string_view data) noexcept {
        return LAW_INTERNAL_ERROR_CHECK(write(getNativeHandler(), data.data(), data.size()));
    }
}