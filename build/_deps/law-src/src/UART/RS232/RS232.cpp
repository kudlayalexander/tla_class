#include "law/UART/RS232/RS232.h"

namespace law::uart::rs232 {
    BlobRet RS232::read() const noexcept {
        BOOST_LEAF_AUTO(availableBytes, utils::getAvailableBytesForRead(getNativeHandler()));
        return (availableBytes == 0) ? read(getDefaultReadCount()) : read(availableBytes);
    }

    BlobRet RS232::read(std::size_t size) const noexcept {
        std::string ret(size, '\0');
        BOOST_LEAF_AUTO(recBytes, Base::read(&ret[0], size));
        ret.resize(recBytes);
        return ret;
    }
}