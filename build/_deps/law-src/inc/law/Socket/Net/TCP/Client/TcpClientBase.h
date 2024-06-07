#pragma once

#include "law/Socket/SendOption.h"
#include "law/Socket/ReadOption.h"
#include "law/Socket/Net/TCP/Base/TcpSocketBase.h"
#include "law/Socket/Net/InternetAddress/InternetAddress.h"

namespace law::socket::net::tcp::detail {
    class TcpClientBase  : public TcpSocketBase {
    private:
        using Base = TcpSocketBase;

    private:
        InternetAddress serverAddress;

    public:
        TcpClientBase();

        TcpClientBase(InternetAddress &&addr);

        TcpClientBase(const InternetAddress &addr);

        TcpClientBase(TcpClientBase &&other) noexcept;

        TcpClientBase &operator=(TcpClientBase &&rhs) noexcept;

        template <typename T>
        void setServerAddress(T &&addr) {
            serverAddress = std::forward<T>(addr);
        }

        [[nodiscard]] const InternetAddress &getServerAddress() const noexcept;

        Status reconnect(const std::chrono::milliseconds &timeout) noexcept;

        Status reconnect() noexcept;

        /**
         * @param timeout - timeout for waiting connection, return X_ETIMEDOUT if reached
         * @return true - On success
         * @return X_EACCES
         * @return X_EPERM
         * @return X_EADDRINUSE
         * @return x_EADDRNOTAVAIL
         * @return X_EAGAIN
         * @return X_EALREADY
         * @return X_ECONNREFUSED
         * @return X_EFAULT
         * @return X_EINPROGRESS
         * @return X_EINTR
         * @return X_EISCONN
         * @return X_ENETUNREACH
         * @return X_ETIMEDOUT
         */
        Status connect(const std::chrono::milliseconds &timeout) const noexcept;

        /**
         * @return true - On success
         * @return X_EACCES
         * @return X_EPERM
         * @return X_EADDRINUSE
         * @return x_EADDRNOTAVAIL
         * @return X_EAGAIN
         * @return X_EALREADY
         * @return X_ECONNREFUSED
         * @return X_EFAULT
         * @return X_EINPROGRESS
         * @return X_EINTR
         * @return X_EISCONN
         * @return X_ENETUNREACH
         * */
        Status connect() const noexcept;

        Status disconnect() noexcept;

        /**
         * @param data - data to send
         * @return true - On success
         * @return X_EAGAIN
         * @return X_EWOULDBLOCK
         * @return X_EALREADY
         * @return X_EBADF
         * @return X_ECONNRESET
         * @return X_EDESTADDRREQ
         * @return X_EFAULT
         * @return X_EINTR
         * @return X_EISCONN
         * @return X_EMSGSIZE
         * @return X_ENOBUFS
         * @return X_ENOMEM
         * @return X_ENOTCONN
         */
        Status send(std::string_view data, const SendOption &so = NoSignal()) const noexcept;

        [[nodiscard]] ReadCountRet read(void *extBuffer, std::size_t readCount, const ReadOption &ro) const noexcept;

        [[nodiscard]] SocketError getSocketError() const noexcept;

    private:
        [[nodiscard]] Status waitConnect(ssize_t connectTimeoutMs) const noexcept;
    };
}
