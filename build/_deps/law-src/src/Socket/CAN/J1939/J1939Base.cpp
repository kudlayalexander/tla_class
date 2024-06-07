#include <linux/can.h>

#include "law/Utils.h"
#include "law/Socket/CAN/J1939/J1939Base.h"

namespace law::socket::can::detail {
    J1939Base::J1939Base(const J1939Address &address) : Base(address) {
        boost::leaf::try_handle_all(
                [this]() -> Status {
                    BOOST_LEAF_CHECK(attach());
                    return {};
                },
                [](RetCode e) {
                    throw std::system_error(std::system_error());
                },
                []() {
                    throw std::runtime_error("J1939 init unknown error");
                }
        );
    }

    Status J1939Base::reattach() noexcept {
        BOOST_LEAF_CHECK(detach());
        return attach();
    }

    Status J1939Base::reattach(const J1939Address &address) noexcept {
        BOOST_LEAF_CHECK(detach());
        return attach(address);
    }

    Status J1939Base::attach(const J1939Address &address) noexcept {
        setCanDevice(address);
        return attach();
    }

    Status J1939Base::attach() noexcept {
        using namespace law::socket::detail;
        BOOST_LEAF_CHECK(makeSocket(SocketType::DGRAM, CAN_J1939));
        BOOST_LEAF_CHECK(bind());
        return {};
    }

    Status J1939Base::detach() noexcept {
        return close();
    }

    Status J1939Base::connect(const J1939Address &destination) const noexcept {
        return LAW_INTERNAL_ERROR_CHECK(
                       ::connect(
                               getNativeHandler(),
                               reinterpret_cast<const sockaddr *>(&destination.getNativeAddress()),
                               sizeof(sockaddr_can)));
    }

    Status J1939Base::applyFilter(const J1939Filter &filter) const noexcept {
        J1939Filter arr[1];
        arr[0] = filter;
        return applyFilter(arr);
    }

    Status J1939Base::applyFilter(J1939FilterSequence filterSequence) const noexcept {
        J1939Filter *filters = filterSequence.data();
        return LAW_INTERNAL_ERROR_CHECK(
                       setsockopt(getNativeHandler(), SOL_CAN_J1939, SO_J1939_FILTER, &filters, filterSequence.size()));
    }

    Status J1939Base::dropAllFilters() const noexcept {
        return applyOption(SocketOptionLevel::J1939, SocketOption::J1939_PROMISC, &utils::detail::kOptionEnableVal);
    }

    Status J1939Base::setSendPriority(J1939SendPriority priority) const noexcept {
        const auto prio = int(priority);
        return applyOption(SocketOptionLevel::J1939, SocketOption::J1939_SEND_PRIO, &prio);
    }

    J1939SendPriorityRet J1939Base::getSendPriority() const noexcept {
        int prio = {};
        BOOST_LEAF_CHECK(readOption(SocketOptionLevel::J1939, SocketOption::J1939_SEND_PRIO, &prio));
        return J1939SendPriority(prio);
    }

    Status J1939Base::broadcast(bool status) const noexcept {
        return applyOption(
                SocketOptionLevel::SOCKET,
                SocketOption::BROADCAST,
                (status ? &utils::detail::kOptionEnableVal : &utils::detail::kOptionDisableVal));
    }

    BoolRet J1939Base::broadcast() const noexcept {
        int state = {};
        BOOST_LEAF_CHECK(readOption(SocketOptionLevel::SOCKET, SocketOption::BROADCAST, &state));
        return bool(state);
    }

    Status J1939Base::send(std::string_view data) const noexcept {
        return LAW_INTERNAL_ERROR_CHECK(write(getNativeHandler(), data.data(), data.size()));
    }

    Status J1939Base::send(std::string_view data, const J1939Address &destination) const noexcept {
        return LAW_INTERNAL_ERROR_CHECK(
                       sendto(
                               getNativeHandler(),
                               data.data(),
                               data.size(),
                               0,
                               reinterpret_cast<const sockaddr *>(&destination.getNativeAddress()),
                               sizeof(sockaddr_can)));
    }

    ReadCountRet J1939Base::read(void *extBuffer, std::size_t readCount) const noexcept {
        const auto recBytes = ::read(getNativeHandler(), extBuffer, readCount);

        if (recBytes == -1) {
            return fromErrno();
        }

        if (recBytes == 0) {
            return boost::leaf::new_error(RetCode::READ_EMPTY_DATA);
        }

        return recBytes;
    }

    ReadResultRet J1939Base::readFrom(void *extBuffer, std::size_t readCount) const noexcept {
        sockaddr_can recAddress{};
        socklen_t addressLength = sizeof(sockaddr_can);

        const auto recBytes =
                ::recvfrom(
                        getNativeHandler(),
                        extBuffer,
                        readCount,
                        0,
                        reinterpret_cast<sockaddr *>(&recAddress),
                        &addressLength);

        if (recBytes == -1) {
            return fromErrno();
        }

        if (recBytes == 0) {
            return boost::leaf::new_error(RetCode::READ_EMPTY_DATA);
        }

        return ReadResult{recAddress, std::size_t(recBytes)};
    }
}