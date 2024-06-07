#pragma once

#include "IField.h"

namespace cp {
    using SerializedConf = CustomError<std::string>;

    template <typename T>
    struct IConfiguration {
        Status parse(std::string_view jsonString) noexcept {
            return impl()->parseImpl(jsonString);
        }

        SerializedConf serialize() noexcept {
            return impl()->serializeImpl();
        }

        auto operator[](std::string_view fieldName) {
            if (!contains(fieldName)) {
                throw std::invalid_argument(fmt::format(FMT_STRING("field {:s} not exist"), fieldName));
            }
            return impl()->getFoldObjectImpl(fieldName);
        }

        [[nodiscard]] bool contains(std::string_view fieldName) const noexcept {
            return constImpl()->containsImpl(fieldName);
        }

    private:
        T *impl() noexcept {
            return static_cast<T *>(this);
        }

        const T *constImpl() const noexcept {
            return static_cast<const T *>(this);
        }
    };
}