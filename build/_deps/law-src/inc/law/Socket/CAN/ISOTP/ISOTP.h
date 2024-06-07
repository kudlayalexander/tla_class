#pragma once

#include "ISOTPBase.h"
#include "law/Types.h"

namespace law::socket::can::isotp {
    template <std::size_t bufSize>
    class StaticISOTP final : public detail::ISOTPBase {
    private:
        using Base = detail::ISOTPBase;
        using ReadBuffer = std::array<char, bufSize + 1>;

    private:
        ReadBuffer readBuffer;

    public:
        using Base::Base;

        [[nodiscard]] BlobViewRet read(std::size_t size = bufSize) noexcept {
            if (size > bufSize) {
                return boost::leaf::new_error(RetCode::NO_AVAILABLE_SPACE);
            }

            auto recBytes = ::read(getNativeHandler(), readBuffer.data(), size);
            if (recBytes == -1) {
                return fromErrno();
            }

            readBuffer[recBytes] = '\0';
            return std::string_view{readBuffer.data(), std::string_view::size_type(recBytes)};
        }
    };

    class ISOTP final : public detail::ISOTPBase {
    private:
        using Base = detail::ISOTPBase;

    public:
        using Base::Base;

        [[nodiscard]] BlobRet read() const noexcept;

        [[nodiscard]] BlobRet read(std::size_t size) const noexcept;
    };
}