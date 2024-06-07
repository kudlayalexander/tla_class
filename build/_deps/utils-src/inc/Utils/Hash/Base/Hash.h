#pragma once

#include <cstdint>
#include <string_view>

struct Hash {
    virtual std::size_t get() const noexcept = 0;

    virtual void push(std::string_view) noexcept = 0;

    virtual ~Hash() noexcept = default;
};

