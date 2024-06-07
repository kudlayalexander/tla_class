#pragma once

#include "law/RetCode/RetCode.h"
#include "law/Socket/ReadOption.h"
#include "law/Socket/Unix/UDP/UdsUdpTypes.h"
#include "law/Socket/Unix/UnixAddress/UnixAddress.h"
#include "law/Socket/Unix/UDP/Base/UdsUdpSocketBase.h"

namespace law::socket::uds::udp::detail {
    class UdsUdpServerBase : public UdsUdpSocketBase {
    private:
        UnixAddress serverAddress;

    public:
        UdsUdpServerBase();

        explicit UdsUdpServerBase(const UnixAddress &srvAddr);

        ~UdsUdpServerBase() noexcept override;

        [[nodiscard]] const UnixAddress &getAddress() const noexcept;

        Status bind() noexcept;

        [[nodiscard]] ReadResultRet
        read(void *extBuffer, std::size_t size, const ReadOption &ro = ReadOption(0)) const noexcept;

    private:
        void openSocket();
    };
}