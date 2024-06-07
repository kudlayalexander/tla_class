#pragma once

#include <cstdint>

#include <ErrorCode/ErrorCode.h>

namespace cp {
    template <typename T>
    using CustomError = ErrorCode::CustomError<T>;

    using Status = ErrorCode::Status;

    enum class RetCode : std::uint8_t {
        GENERIC = 0, /* Configuration generic error */

        FIELD_NOT_EXIST = 20, /* Configuration field not exist */
        EMPTY_FIELD = 21, /* Configuration field is empty */

        INVALID_TYPE = 40, /* Configuration field has wrong type */
        INVALID_RANGE = 41, /* Configuration field value out of range */
        INVALID_LENGTH = 42, /* Configuration field value has invalid length */
        UNEXPECTED_VALUE = 43, /* Configuration field value has unexpected value */

        SERIALIZE_ERROR = 60, /* Configuration serialize error */
        DESERIALIZE_ERROR = 61, /* Configuration deserialize (parse) error */

        CONTAINER_MAX_COUNT_EXCEED = 80, /* Max count of entities in container field was exceeded */
    };

    boost::leaf::error_id newError(RetCode rc);
}