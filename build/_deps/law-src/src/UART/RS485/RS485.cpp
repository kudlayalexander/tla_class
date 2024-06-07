#include "law/UART/RS485/RS485.h"

namespace law::uart::rs485 {
    BlobRet RS485::read() const noexcept {
        BOOST_LEAF_AUTO(availableBytes, utils::getAvailableBytesForRead(getNativeHandler()));
        return (availableBytes == 0) ? read(getDefaultReadCount()) : read(availableBytes);
    }

    BlobRet RS485::read(std::size_t size) const noexcept {
        std::string ret(size, '\0');
        BOOST_LEAF_AUTO(recBytes, Base::read(&ret[0], size));
        ret.resize(recBytes);
        return ret;
    }
}