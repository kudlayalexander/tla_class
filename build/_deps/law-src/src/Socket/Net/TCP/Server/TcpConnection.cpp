#include "law/Socket/Net/TCP/Server/TcpConnection.h"

namespace law::socket::net::tcp {
    [[nodiscard]] BlobRet TcpConnection::read(const ReadOption &ro) noexcept {
        BOOST_LEAF_AUTO(availableBytes, utils::getAvailableBytesForRead(getNativeHandler()));
        return (availableBytes == 0) ? read(kDefaultReadCount, ro) : read(availableBytes, ro);
    }

    [[nodiscard]] BlobRet TcpConnection::read(std::size_t size, const ReadOption &ro) noexcept {
        std::string ret(size, '\0');
        BOOST_LEAF_AUTO(recBytes, Base::read(&ret[0], size, ro.getAsIntegral()));
        ret.resize(recBytes);
        return ret;
    }
}