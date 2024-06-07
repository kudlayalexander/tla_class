#include "law/Socket/Unix/UDP/Client/UdsUdpClient.h"

namespace law::socket::uds::udp {
    [[nodiscard]] BlobRet UdsUdpClient::read(const ReadOption &ro) const noexcept {
        BOOST_LEAF_AUTO(availableBytes, utils::getAvailableBytesForRead(getNativeHandler()));
        return (availableBytes == 0) ? read(getDefaultReadCount(), ro) : read(availableBytes, ro);
    }

    [[nodiscard]] BlobRet UdsUdpClient::read(std::size_t size, const ReadOption &ro) const noexcept {
        std::string ret(size, '\0');
        BOOST_LEAF_AUTO(recBytes, Base::read(&ret[0], size, ro));
        ret.resize(recBytes);
        return ret;
    }
}