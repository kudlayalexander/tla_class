#include "Utils/Hash/CRC32/CRC32.h"

namespace Utils {
    [[nodiscard]] std::size_t CRC32::get() const noexcept {
        return crc;
    }

    void CRC32::push(std::string_view str) noexcept {
        crc = calculateHash(str);
    }

    bool CRC32::operator==(const CRC32 &crc32) const noexcept {
        return crc == crc32.crc;
    }

    std::uint32_t CRC32::calculateHash(std::string_view str) noexcept {
        return calculateHash(str.cbegin(), str.cend());
    }
}

