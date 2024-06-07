#pragma once

#include <cstdint>
#include <compare>
#include <concepts>

namespace law {
    namespace detail {
        template <std::integral T>
        class FlagValue {
        private:
            T flag = 0;

        public:
            constexpr FlagValue() noexcept = default;

            FlagValue(T flags) noexcept: flag(flags) {}

            FlagValue(const FlagValue &fv) noexcept: flag(fv.getAsIntegral()) {}

            FlagValue(FlagValue &&fv) noexcept: flag(fv.getAsIntegral()) {}

            FlagValue &operator=(const FlagValue &fv) noexcept {
                if (&fv != this) {
                    set(fv.flag);
                }
                return *this;
            }

            FlagValue &operator=(FlagValue &&fv) noexcept {
                if (&fv != this) {
                    set(fv.flag);
                }
                return *this;
            }

            FlagValue &operator=(T flags) noexcept {
                set(flags);
                return *this;
            }

            void set(T flags) noexcept {
                flag = flags;
            }

            void set(FlagValue fv) noexcept {
                flag = fv.getAsIntegral();
            }

            [[nodiscard]] T getAsIntegral() const noexcept {
                return flag;
            }

            bool contains(FlagValue fv) const noexcept {
                return operator&(fv);
            }

            bool contains(T value) const noexcept {
                return operator&(value);
            }

            FlagValue &operator&=(FlagValue fv) noexcept {
                return operator&=(fv.flag);
            }

            FlagValue &operator&=(T flags) noexcept {
                flag &= flags;
                return *this;
            }

            FlagValue &operator|=(FlagValue fv) noexcept {
                return operator|=(fv.flag);
            }

            FlagValue &operator|=(T flags) noexcept {
                flag |= flags;
                return *this;
            }

            FlagValue operator&(const FlagValue &fv) const noexcept {
                return FlagValue(flag & fv.flag);
            }

            FlagValue operator&(T flags) const noexcept {
                return FlagValue(flag & flags);
            }

            FlagValue operator|(const FlagValue &fv) const noexcept {
                return FlagValue(flag | fv.flag);
            }

            FlagValue operator|(T flags) const noexcept {
                return FlagValue(flag | flags);
            }

            FlagValue &operator~() noexcept {
                flag = ~flag;
                return *this;
            }

            explicit operator bool() const noexcept {
                return flag;
            }

            auto operator<=>(const FlagValue &) const noexcept = default;
        };
    }

    using IntFlagValue = detail::FlagValue<int>;
    using ShortFlagValue = detail::FlagValue<short>;
    using UInt32FlagValue = detail::FlagValue<std::uint32_t>;
}