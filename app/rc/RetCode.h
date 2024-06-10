#pragma once

#include <array>

#include <ErrorCode/ErrorCode.h>

enum class RetCode : std::uint8_t {
    ROM_CONFIGURATION_CORRUPT,
    ROM_CONFIGURATION_LOAD_ERROR,
    ROM_CONFIGURATION_SAVE_ERROR,
    ROM_CONFIGURATION_RESTORE_ERROR,
    ROM_CONFIGURATION_SERIALIZE_ERROR,
    ROM_CONFIGURATION_FIELD_NOT_PRESENCE,
    ROM_CONFIGURATION_STRING_FIELD_LENGTH_TOO_BIG,
    ROM_CONFIGURATION_FIELD_VALUE_RANGE_VIOLATION,
    ROM_CONFIGURATION_UNEXPECTED_FIELD_VALUE,
    ROM_CONFIGURATION_INCORRECT_FIELD_TYPE,

    MAX_VALUE
};

template <typename T>
using CustomError = ErrorCode::CustomError<T>;

using Status = ErrorCode::Status;

static boost::leaf::error_id newError(RetCode rc) noexcept {
    return boost::leaf::new_error(rc);
}

static boost::leaf::error_id newError(RetCode rc, std::string_view reason) noexcept {
    return boost::leaf::new_error(rc, reason);
}

static constexpr bool isRomConfigurationError(RetCode rc) {
    return (rc >= RetCode::ROM_CONFIGURATION_CORRUPT && rc <= RetCode::ROM_CONFIGURATION_INCORRECT_FIELD_TYPE);
}

static constexpr std::string_view rcToString(RetCode rc) noexcept {
    constexpr std::array<std::string_view, std::size_t(RetCode::MAX_VALUE)> retCodeStrings = {
            "Config: configuration corrupt",
            "Config: configuration load error",
            "Config: configuration save error",
            "Config: configuration restore error",
            "Config: configuration serialize error",
            "Config: configuration field not present",
            "Config: configuration string field length is too big",
            "Config: configuration field value range violation",
            "Config: configuration unexpected field value",
            "Config: configuration incorrect field type"
    };
    return (std::size_t(rc) < std::size_t(RetCode::MAX_VALUE)) ? retCodeStrings[std::size_t(rc)] : "Unknown Error";
}