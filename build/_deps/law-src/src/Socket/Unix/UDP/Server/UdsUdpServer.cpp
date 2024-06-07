#include "law/Socket/Unix/UDP/Server/UdsUdpServer.h"

namespace law::socket::uds::udp {
    ClientPayloadRet UdsUdpServer::read(const ReadOption &ro) noexcept {
        BOOST_LEAF_AUTO(availableBytes, utils::getAvailableBytesForRead(getNativeHandler()));
        return (availableBytes == 0) ? read(getDefaultReadCount(), ro) : read(availableBytes, ro);
    }

    ClientPayloadRet UdsUdpServer::read(std::size_t size, const ReadOption &ro) const noexcept {
        std::string payload(size, '\0');

        BOOST_LEAF_AUTO(readResult, Base::read(&payload[0], size, ro));
        auto &[clientAddress, recBytes] = readResult;

        payload.resize(recBytes);
        return makeClientPayload(clientAddress, std::move(payload));
    }

    ClientPayload UdsUdpServer::makeClientPayload(const sockaddr_un &nativeAddress, std::string &&payload) noexcept {
        ClientPayload ret;
        ret.address.setNativeAddress(nativeAddress);
        ret.blob = std::move(payload);
        return ret;
    }
}