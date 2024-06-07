#pragma once

#include <ranges>
#include <concepts>
#include <string_view>

#include <etl/string_view.h>

namespace cp {
    template <typename T>
    concept Bool = std::same_as<bool, T>;

    template <typename T>
    concept Numeric = ((std::integral<T> || std::floating_point<T>) && !Bool<T>);

    template <typename T>
    concept String = (std::convertible_to<T, std::string_view> || std::convertible_to<T, etl::string_view>);

    template <typename T>
    concept Array = (std::ranges::random_access_range<T> && !String<T>);
}