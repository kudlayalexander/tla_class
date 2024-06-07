#include "law/Socket/CAN/J1939/J1939.h"

namespace law::socket::can {
    [[nodiscard]] BlobRet J1939::read(std::size_t size) const noexcept {
        std::string ret(size, '\0');
        BOOST_LEAF_AUTO(recBytes, Base::read(&ret[0], size));
        ret.resize(recBytes);
        return ret;
    }

    [[nodiscard]] ReadResultBlobRet J1939::readFrom(std::size_t size) const noexcept {
        std::string ret(size, '\0');
        BOOST_LEAF_AUTO(readResult, Base::readFrom(&ret[0], size));
        ret.resize(readResult.recBytes);
        return ReadResultBlob{readResult.sourceAddress, std::move(ret)};
    }
}