#pragma once

#include <atomic>
#include <algorithm>

#include <etl/string.h>
#include <law/Signal/Signal.h>

namespace utils {
    /**
     * @brief Is a special string-class, which may be used as template argument
     *      Example: template <TemplateString str>
     * On client-side of code it looks like:
     *      Example: Type<"string"> t;
     * */
    template <std::size_t N>
    struct TemplateString {
        constexpr TemplateString(const char (&str)[N]) {
            std::copy_n(str, N, value);
        }

        char value[N];
        std::size_t len = (N - 1);
    };

    /**
     * @brief Convert etl::string to std::string_view
     * */
    template <std::size_t strSize>
    std::string_view etlStringToStrView(const etl::string<strSize> &str) noexcept {
        return {str.data(), str.size()};
    }

    /**
     * @brief Special tool-class, method 'get' becomes true when any of UNIX signals, passed to constructor, happens
     * */
    class ExitFlag {
    private:
        std::atomic_flag exitFlag{false};

    public:
        ExitFlag(auto ...signals) {
            law::signal::SignalHandler::getInstance().subscribe(
                    [this](auto...) {
                        exitFlag.test_and_set();
                    },
                    signals...
            );
        }

        [[nodiscard]] bool get() const noexcept {
            return exitFlag.test();
        }
    };

    /**
     * @brief concatenate a set of std::string_view's at compile time
     */
    template <std::string_view const &... Strs>
    struct StrViewJoin {
        static constexpr auto impl() noexcept {
            constexpr std::size_t len = (Strs.size() + ... + 0);
            std::array<char, len + 1> arr{};
            auto append = [i = 0, &arr](auto const &s) mutable {
                for (auto c: s) arr[i++] = c;
            };
            (append(Strs), ...);
            arr[len] = 0;
            return arr;
        }

        static constexpr auto array = impl();
        static constexpr std::string_view value{array.data(), array.size() - 1};
    };

    /**
     * @brief A more short record of @class StrViewJoin
     * Is analogue of *_v constexpr variables from <type_traits>
     */
    template <std::string_view const &... Strs>
    static constexpr auto StrViewJoinValue = StrViewJoin<Strs...>::value;
}