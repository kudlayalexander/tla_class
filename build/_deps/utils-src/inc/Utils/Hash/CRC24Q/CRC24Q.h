#pragma once

#include "Utils/Hash/Base/Hash.h"

namespace Utils {
//    struct CRC24Q {
//        static bool verify(std::string_view message) noexcept {
//            return crc24q(message) == 0;
//        }
//
//        static std::uint32_t crc24q(std::string_view message) noexcept {
//            static constexpr std::array table = {
//                    0x00000000, 0x01864CFB, 0x038AD50D, 0x020C99F6, 0x0793E6E1, 0x0615AA1A, 0x041933EC, 0x059F7F17,
//                    0x0FA18139, 0x0E27CDC2, 0x0C2B5434, 0x0DAD18CF, 0x083267D8, 0x09B42B23, 0x0BB8B2D5, 0x0A3EFE2E
//            };
//
//            std::uint32_t crc = 0;
//            for (const std::uint32_t it: message) {
//                crc ^= (it << 16);
//                crc = (crc << 4) ^ table[(crc >> 20) & 0x0F];
//                crc = (crc << 4) ^ table[(crc >> 20) & 0x0F];
//            }
//
//            return (crc & 0xFFFFFF);
//        }
//    };

    class CRC24Q : public Hash {
    private:
        std::uint32_t crc;

    public:
        CRC24Q() noexcept: crc(0) { }

        explicit CRC24Q(std::string_view str) noexcept: crc(calculateHash(str)) { }

        [[nodiscard]] std::size_t get() const noexcept override;

        void push(std::string_view) noexcept override;

        bool operator==(const CRC24Q &crc24) const noexcept;

    private:
        static std::uint32_t calculateHash(std::string_view message) noexcept {
            static constexpr std::array table = {
                    0x00000000, 0x01864CFB, 0x038AD50D, 0x020C99F6, 0x0793E6E1, 0x0615AA1A, 0x041933EC, 0x059F7F17,
                    0x0FA18139, 0x0E27CDC2, 0x0C2B5434, 0x0DAD18CF, 0x083267D8, 0x09B42B23, 0x0BB8B2D5, 0x0A3EFE2E
            };

            std::uint32_t crc = 0;
            for (const std::uint32_t it: message) {
                crc ^= (it << 16);
                crc = (crc << 4) ^ table[(crc >> 20) & 0x0F];
                crc = (crc << 4) ^ table[(crc >> 20) & 0x0F];
            }

            return (crc & 0xFFFFFF);
        }
    };
}