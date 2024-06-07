#pragma once

#include "law/Types.h"
#include "law/FlagValue.h"

namespace law::file::utils {
    struct FilePermissionTest : IntFlagValue {
        using IntFlagValue::IntFlagValue;

        FilePermissionTest operator|(const FilePermissionTest &rhs) const noexcept;
    };

    struct ReadPermissionTest : FilePermissionTest {
        ReadPermissionTest();
    };

    struct WritePermissionTest : FilePermissionTest {
        WritePermissionTest();
    };

    struct ExecutePermissionTest : FilePermissionTest {
        ExecutePermissionTest();
    };

    BoolRet fileExist(std::string_view path) noexcept;

    BoolRet testPermission(std::string_view path, const FilePermissionTest &permission) noexcept;
}