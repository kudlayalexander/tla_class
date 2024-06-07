#include "law/Pipe/Pipe.h"

namespace law::pipe {
    [[nodiscard]] BlobRet Pipe::read() const noexcept {
        BOOST_LEAF_AUTO(pipeSize, getPipeSize());
        return read(pipeSize);
    }

    [[nodiscard]] BlobRet Pipe::read(std::size_t size) const noexcept {
        std::string ret(size, '\0');
        BOOST_LEAF_AUTO(recBytes, Base::read(ret.data(), size));
        ret.resize(recBytes);
        return ret;
    }
}