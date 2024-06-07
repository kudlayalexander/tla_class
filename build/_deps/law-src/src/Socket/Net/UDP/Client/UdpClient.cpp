#include "law/Socket/Net/UDP/Client/UdpClient.h"

namespace law::socket::net::udp {
    BlobRet UdpClient::read(const ReadOption &ro) const noexcept {
        BOOST_LEAF_AUTO(availableBytes, utils::getAvailableBytesForRead(getNativeHandler()));
        return (availableBytes == 0) ? read(getDefaultReadCount(), ro) : read(availableBytes, ro);
    }

    BlobRet UdpClient::read(std::size_t size, const ReadOption &ro) const noexcept {
        std::string ret(size, '\0');
        BOOST_LEAF_AUTO(recBytes, Base::read(&ret[0], size, ro.getAsIntegral()));
        ret.resize(recBytes);
        return ret;
    }
}