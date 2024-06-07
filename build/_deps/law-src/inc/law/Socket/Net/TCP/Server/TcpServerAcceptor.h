#pragma once

#include <sys/poll.h>

#include "TcpServerBase.h"
#include "law/RetCode/RetCode.h"
#include "law/Socket/Net/TCP/Client/TcpClient.h"

namespace law::socket::net::tcp::detail {
    template <typename Connection>
    class TcpServerAcceptor : public TcpServerBase {
    public:
        using ConnectionRet = ErrorCode::CustomError<Connection>;

    public:
        using TcpServerBase::TcpServerBase;

        [[nodiscard]] ConnectionRet accept(std::chrono::milliseconds timeout, const AcceptOption &ao = AcceptOption(0)) const noexcept {
            int nativeTimeout = -1;
            if (timeout != std::chrono::milliseconds::max()) {
                nativeTimeout = int(timeout.count());
            }

            pollfd pfd{.fd = getNativeHandler(), .events = POLLIN};
            switch (poll(&pfd, 1, nativeTimeout)) {
                case -1: {
                    return fromErrno();
                }
                case 0: {
                    return boost::leaf::new_error(RetCode::TIMEOUT);
                }
                default: {
                    return accept(ao);
                }
            }
        }

        [[nodiscard]] ConnectionRet accept(const AcceptOption &ao = AcceptOption(0)) const noexcept {
            BOOST_LEAF_AUTO(connectionFd, acceptConnection(ao));
            return Connection(connectionFd);
        }
    };
}
