#pragma once

#include <ErrorCode/ErrorCode.h>

#include "Permission.h"

namespace law::file {
    using FileSize = ErrorCode::CustomError<std::size_t>;
    using FilePermissions = ErrorCode::CustomError<Permission>;
}