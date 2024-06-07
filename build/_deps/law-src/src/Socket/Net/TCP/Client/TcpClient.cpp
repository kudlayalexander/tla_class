#include "law/Socket/Net/TCP/Client/TcpClient.h"

namespace law::socket::net::tcp {
    BlobRet TcpClient::read(const ReadOption &ro) const noexcept {
        BOOST_LEAF_AUTO(availableBytes, utils::getAvailableBytesForRead(getNativeHandler()));
        return (availableBytes == 0) ? read(getDefaultReadCount(), ro) : read(availableBytes, ro);
    }

    BlobRet TcpClient::read(std::size_t size, const ReadOption &ro) const noexcept {
        std::string ret(size, '\0');
        BOOST_LEAF_AUTO(recBytes, Base::read(&ret[0], size, ro.getAsIntegral()));
        ret.resize(recBytes);
        return ret;
    }
}