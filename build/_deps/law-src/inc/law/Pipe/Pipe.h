#pragma once

#include "Base/PipeBase.h"
#include "law/RetCode/RetCode.h"

namespace law::pipe {
    template <std::size_t bufSize>
    class StaticPipe final : public detail::PipeBase {
    private:
        using Base = detail::PipeBase;

        using ReadBuffer = std::array<char, bufSize + 1>;

        using Base::read;

    private:
        ReadBuffer readBuffer;

    public:
        [[nodiscard]] BlobViewRet read() noexcept {
            BOOST_LEAF_AUTO(pipeSize, this->getPipeSize());
            return read((pipeSize > bufSize ? bufSize : pipeSize));
        }

        [[nodiscard]] BlobViewRet read(std::size_t size) noexcept {
            if (size > bufSize) {
                return boost::leaf::new_error(RetCode::NO_AVAILABLE_SPACE);
            }

            BOOST_LEAF_AUTO(recBytes, Base::read(readBuffer.data(), size));

            readBuffer[recBytes] = '\0';
            return std::string_view{readBuffer.data(), std::string_view::size_type(recBytes)};
        }
    };

    class Pipe final : public detail::PipeBase {
    private:
        using Base = detail::PipeBase;

    public:
        [[nodiscard]] BlobRet read() const noexcept;

        [[nodiscard]] BlobRet read(std::size_t size) const noexcept;
    };

    using ExtBufferPipe = detail::PipeBase;
}