#include <sys/socket.h>

#include "law/RetCode/RetCode.h"
#include "law/Socket/Net/TCP/Server/TcpConnectionBase.h"

namespace law::socket::net::tcp::detail {
    TcpConnectionBase::TcpConnectionBase(int fd) noexcept: fd::FileDescriptor(fd) { }

    Status TcpConnectionBase::send(std::string_view data, const SendOption &so) const noexcept {
        if (::send(getNativeHandler(), data.data(), data.size(), so.getAsIntegral()) == -1) {
            return fromErrno();
        }

        return {};
    }

    [[nodiscard]] ReadCountRet TcpConnectionBase::read(void *extBuffer, std::size_t readCount, const ReadOption &ro) const noexcept {
        const auto recBytes = recv(getNativeHandler(), extBuffer, readCount, ro.getAsIntegral());

        if (recBytes == -1) {
            return fromErrno();
        }

        if (recBytes == 0) {
            return boost::leaf::new_error(RetCode::READ_EMPTY_DATA);
        }

        return recBytes;
    }

    Status TcpConnectionBase::destroy() noexcept {
        return close();
    }
}