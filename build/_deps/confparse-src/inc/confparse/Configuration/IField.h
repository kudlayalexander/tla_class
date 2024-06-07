#pragma once

#include <boost/type_index.hpp>

#include <Logger/Logger.h>

#include "Concepts.h"
#include "confparse/RetCode.h"

namespace cp {
    template <typename T>
    struct IField {
    private:
        static constexpr char moduleName[] = "ConfigModule";

    public:
        template <Bool V>
        V get(V defaultValue) const noexcept {
            using namespace boost::leaf;
            return try_handle_all(
                    [this, &defaultValue]() -> CustomError<V> {
                        return getValue<V>(defaultValue);
                    },
                    [&defaultValue]() {
                        LOGGER_ERROR(moduleName,
                                     "unknown error -> would be used default value '{}'",
                                     defaultValue);
                        return std::forward<V>(defaultValue);
                    }
            );
        }

        template <Numeric V>
        V get(V defaultValue,
              V minVal = std::numeric_limits<V>::min(),
              V maxVal = std::numeric_limits<V>::max()) const noexcept {
            using namespace boost::leaf;
            return try_handle_all(
                    [this, &defaultValue, minVal, maxVal]() -> CustomError<V> {
                        const auto retVal = getValue<V>(defaultValue);
                        BOOST_LEAF_CHECK(inRange(retVal, minVal, maxVal));
                        return retVal;
                    },
                    [&defaultValue, minVal, maxVal](match<RetCode, RetCode::INVALID_RANGE>) {
                        LOGGER_WARNING(moduleName,
                                       "invalid range of field (should be {} <= val <= {}) -> "
                                       "would be used default value '{}'",
                                       minVal,
                                       maxVal,
                                       defaultValue);
                        return std::forward<V>(defaultValue);
                    },
                    [&defaultValue]() {
                        LOGGER_ERROR(moduleName,
                                     "unknown error -> would be used default value '{}'",
                                     defaultValue);
                        return std::forward<V>(defaultValue);
                    }
            );
        }

        template <String V>
        V get() const noexcept {
            using namespace boost::leaf;
            return get<V>("");
        }

        template <String V, typename F, typename ...Args>
        V get(F defaultValue, const Args &...args) const noexcept {
            using namespace boost::leaf;
            return try_handle_all(
                    [this, defaultValue, &args...]() -> CustomError<V> {
                        const auto retVal = getValue<V>(defaultValue);
                        if constexpr (sizeof...(args) > 0) {
                            BOOST_LEAF_CHECK(isStringAllowValueVarargs(retVal, defaultValue, args...));
                        }
                        return retVal;
                    },
                    [&defaultValue, &args...](match<RetCode, RetCode::UNEXPECTED_VALUE>) {
                        std::array<F, sizeof...(args) + 1> allowValues{defaultValue, args...};
                        LOGGER_WARNING(moduleName,
                                       "incorrect value, allow values: '{}' -> would be used default value {}",
                                       fmt::join(allowValues, ", "),
                                       defaultValue);
                        return std::forward<V>(defaultValue);
                    },
                    [&defaultValue]() {
                        LOGGER_ERROR(moduleName,
                                     "unknown error -> would be used default value '{}'",
                                     defaultValue);
                        return std::forward<V>(defaultValue);
                    }
            );
        }

        template <String V, std::ranges::sized_range R>
        requires ( !String<R> )
        V get(V defaultValue, const R &allowValues) const noexcept {
            using namespace boost::leaf;
            return try_handle_all(
                    [this, defaultValue, &allowValues]() -> CustomError<V> {
                        const auto retVal = getValue<V>(defaultValue);
                        if (!allowValues.empty()) {
                            BOOST_LEAF_CHECK(isStringAllowValueArray(retVal, defaultValue, allowValues));
                        }
                        return retVal;
                    },
                    [&defaultValue, &allowValues](match<RetCode, RetCode::UNEXPECTED_VALUE>) {
                        LOGGER_WARNING(moduleName,
                                       "incorrect value, allow values: '{}' -> would be used default value {}",
                                       fmt::join(allowValues, ", "),
                                       defaultValue);
                        return std::forward<V>(defaultValue);
                    },
                    [&defaultValue]() {
                        LOGGER_ERROR(moduleName,
                                     "unknown error -> would be used default value '{}'",
                                     defaultValue);
                        return std::forward<V>(defaultValue);
                    }
            );
        }

        T operator[](std::string_view fieldName) {
            if (!contains(fieldName)) {
                throw std::invalid_argument(fmt::format(FMT_STRING("field {:s} not exist"), fieldName));
            }
            return impl()->getFoldObjectImpl(fieldName);
        }

        [[nodiscard]] bool contains(std::string_view fieldName) const noexcept {
            return constImpl()->containsImpl(fieldName);
        }

    private:
        template <typename V, typename F>
        V getValue(F defaultValue) const noexcept {
            using namespace boost::leaf;
            return try_handle_all(
                    [this]() -> CustomError<V> {
                        return constImpl()->template getImpl<V>();
                    },
                    [&defaultValue](match<RetCode, RetCode::INVALID_TYPE>) {
                        LOGGER_WARNING(moduleName,
                                       "can't get value of field "
                                       "(incorrect type specified: expected '{:s}') -> "
                                       "would be used default value '{}'",
                                       boost::typeindex::type_id<V>().pretty_name(),
                                       defaultValue);
                        return std::forward<V>(defaultValue);
                    },
                    [&defaultValue]() {
                        LOGGER_ERROR(moduleName,
                                     "unknown error -> would be used default value '{}'",
                                     defaultValue);
                        return std::forward<V>(defaultValue);
                    }
            );
        }

        T *impl() noexcept {
            return static_cast<T *>(this);
        }

        const T *constImpl() const noexcept {
            return static_cast<const T *>(this);
        }

        template <typename V>
        static Status inRange(const V &val, V min, V max) noexcept {
            if ((val <= min) || (val >= max)) {
                return newError(RetCode::INVALID_RANGE);
            }
            return {};
        }

        template <typename V, typename F, typename ...Args>
        static Status isStringAllowValueVarargs(const V &val, const F &head, const Args &...tail) noexcept {
            if (val == head) {
                return {};
            }

            if constexpr (sizeof...(tail) != 0) {
                return isStringAllowValueVarargs(val, tail...);
            }

            return newError(RetCode::UNEXPECTED_VALUE);
        }

        template <typename V, typename F, std::ranges::range R>
        static Status isStringAllowValueArray(const V &val, const F &defaultValue, const R &allowValues) noexcept {
            return (
                    (std::ranges::find(allowValues, val) == allowValues.cend() && val != defaultValue) ?
                    newError(RetCode::UNEXPECTED_VALUE) :
                    Status());
        }
    };
}