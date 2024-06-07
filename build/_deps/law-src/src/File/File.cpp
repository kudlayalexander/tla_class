#include "law/File/File.h"

namespace law::file {
    [[nodiscard]] BlobRet File::read() const noexcept {
        BOOST_LEAF_AUTO(fileSize, getFileSize());
        return read(fileSize);
    }

    [[nodiscard]] BlobRet File::read(std::size_t size) const noexcept {
        std::string ret(size, '\0');
        BOOST_LEAF_AUTO(recBytes, Base::read(ret.data(), size));
        ret.resize(recBytes);
        return ret;
    }
}