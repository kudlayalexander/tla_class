#pragma once

#include <etl/string.h>

#include "law/Socket/SendOption.h"
#include "law/Socket/Unix/Base/UnixSocketBase.h"
#include "law/Socket/Unix/UnixAddress/UnixAddress.h"

namespace law::socket::uds::udp::detail {
    class UdsUdpSocketBase : public uds::detail::UnixSocketBase {
    private:
        using UnixPath = etl::string<sizeof(sockaddr_un)>;

    public:
        Status send(std::string_view data, const UnixAddress &addr, const SendOption &so = Confirm()) const noexcept;

    protected:
        Status makeSocket() noexcept;

    protected:
        static UnixPath makeRandomUnixPath() noexcept;
    };
}