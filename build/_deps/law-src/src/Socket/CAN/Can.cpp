#include <sys/ioctl.h>
#include <linux/can/raw.h>

#include "law/Utils.h"
#include "law/Socket/CAN/Can.h"
#include "law/RetCode/RetCode.h"

static constexpr std::uint8_t kDlc2LenTable[] = {
        0, 1, 2, 3, 4, 5, 6, 7,
        8, 12, 16, 20, 24, 32, 48, 64
};

static std::uint8_t canDlc2Len(std::uint8_t dlc) noexcept {
    return kDlc2LenTable[dlc & 0x0F];
}

static constexpr std::uint8_t kLen2DlcTable[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8,
        9, 9, 9, 9,
        10, 10, 10, 10,
        11, 11, 11, 11,
        12, 12, 12, 12,
        13, 13, 13, 13, 13, 13, 13, 13,
        14, 14, 14, 14, 14, 14, 14, 14,
        14, 14, 14, 14, 14, 14, 14, 14,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15
};

static std::uint8_t canLen2Dlc(std::uint8_t len) noexcept {
    return ((len <= 64) ? kLen2DlcTable[len] : 0x0F);
}

namespace law::socket::can {
    Can::Can(CanMode m) noexcept: mode(m) {}

    Can::Can(const CanDevice &device, CanMode m) : Base(device), mode(m) {
        if (!init()) {
            throw std::system_error(errno, std::system_category());
        }
    }

    Can::Can(CanDevice &&device, CanMode m) : Base(std::move(device)), mode(m) {
        if (!init()) {
            throw std::system_error(errno, std::system_category());
        }
    }

    Status Can::init() noexcept {
        using namespace law::socket::detail;
        BOOST_LEAF_CHECK(makeSocket(SocketType::RAW, CAN_RAW));
        if (mode == CanMode::CAN_FD) {
            BOOST_LEAF_AUTO(isSupported, isCanfdSupported());
            if (!isSupported) {
                return boost::leaf::new_error(RetCode::NOT_SUPPORTED);
            }

            static constexpr int enable = 1;
            BOOST_LEAF_CHECK(applyOption(SocketOptionLevel::RAW_CAN, CAN_RAW_FD_FRAMES, &enable));
        }

        return {};
    }

    Status Can::send(CanFrameSequence frameSequence) const noexcept {
        for (const auto &it: frameSequence) {
            BOOST_LEAF_CHECK(send(it));
        }
        return {};
    }

    Status Can::send(const CanFrame &frame) const noexcept {
        auto canfdFrame = makeCanfdFrame(frame);

        if (mode == CanMode::CAN_FD) {
            canfdFrame.len = canDlc2Len(canLen2Dlc(canfdFrame.len));
        }

        if (::write(getNativeHandler(), &canfdFrame, int(mode)) != int(mode)) {
            return fromErrno();
        }

        return {};
    }

    [[nodiscard]] CanFrameRet Can::read() const noexcept {
        canfd_frame frame{};

        auto recBytes = ::read(getNativeHandler(), &frame, CANFD_MTU);
        if (recBytes == 0) {
            return boost::leaf::new_error(RetCode::READ_EMPTY_DATA);
        }

        if ((recBytes != CAN_MTU) || (recBytes != CANFD_MTU)) {
            return fromErrno();
        }

        return makeCanFrame(frame);
    }

    Status Can::applyFilter(CanFilter filter) const noexcept {
        return applyFilter({&filter, 1});
    }

    Status Can::applyFilter(CanFilterSequence filterSequence) const noexcept {
        CanFilter *filters = filterSequence.data();
        return LAW_INTERNAL_ERROR_CHECK(
                       setsockopt(
                               getNativeHandler(),
                               SOL_CAN_RAW,
                               CAN_RAW_FILTER,
                               &filters,
                               filterSequence.size()
                       )
               );
    }

    Status Can::dropAllFilters() const noexcept {
        return LAW_INTERNAL_ERROR_CHECK(
                       setsockopt(
                               getNativeHandler(),
                               SOL_CAN_RAW,
                               CAN_RAW_FILTER,
                               nullptr,
                               0
                       )
               );
    }

    BoolRet Can::isCanfdSupported() const noexcept {
        ifreq ifr{};

        std::strncpy(ifr.ifr_name, getCanDevice().getInterfaceName().data(), sizeof(ifr.ifr_name));
        if (ioctl(getNativeHandler(), SIOCGIFMTU, &ifr) == -1) {
            return fromErrno();
        }

        return (ifr.ifr_mtu == CANFD_MTU);
    }

    canfd_frame Can::makeCanfdFrame(const CanFrame &frame) noexcept {
        canfd_frame canfdFrame{};
        std::memset(&canfdFrame, 0, sizeof(canfd_frame));

        canfdFrame.can_id = frame.id;
        canfdFrame.len = frame.data.size();
        canfdFrame.flags = 0;
        std::memcpy(canfdFrame.data, frame.data.data(), frame.data.size());

        return canfdFrame;
    }

    CanFrame Can::makeCanFrame(const canfd_frame &frame) noexcept {
        CanFrame msg;
        msg.id = frame.can_id;
        msg.data.assign(reinterpret_cast<const char *>(frame.data), frame.len);
        return msg;
    }
}