#include "law/Utils.h"
#include "law/Socket/CAN/ISOTP/ISOTP.h"

namespace law::socket::can::isotp {
    [[nodiscard]] BlobRet ISOTP::read() const noexcept {
        return read(getDefaultReadCount());
    }

    [[nodiscard]] BlobRet ISOTP::read(std::size_t size) const noexcept {
        std::string ret(size, '\0');
        auto recBytes = ::read(getNativeHandler(), ret.data(), size);
        if (recBytes == -1) {
            return fromErrno();
        }

        ret.resize(recBytes);
        return ret;
    }
}