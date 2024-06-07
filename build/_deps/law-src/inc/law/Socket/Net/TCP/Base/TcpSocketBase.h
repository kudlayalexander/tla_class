#pragma once

#include "law/Socket/Net/TCP/TcpTypes.h"
#include "law/Socket/Net/TCP/TcpOption.h"
#include "law/Socket/Net/Base/NetSocketBase.h"

namespace law::socket::net::tcp::detail {
    class TcpSocketBase : public net::detail::NetSocketBase {
    private:
        using Base = net::detail::NetSocketBase;

    protected:
        void openSocket();

        Status makeSocket() noexcept;

    public:
        Status applyOption(SocketOptionLevel level, TcpOption tcpOption, const auto *optionValue) const noexcept {
            return Base::applyOption(level, int(tcpOption), optionValue);
        }

        Status readOption(SocketOptionLevel level, TcpOption tcpOption, auto *optionValue) const noexcept {
            return Base::readOption(level, int(tcpOption), optionValue);
        }

        Status setupKeepalive(std::chrono::seconds startThreshold,
                              std::uint32_t repeats,
                              std::chrono::seconds delayBetweenRepeats) const noexcept;

        Status disableNagleAlgorithm() const noexcept;

        Status reuseAddress() const noexcept;

        Status reusePort() const noexcept;
    };
}
