#include <unistd.h>

#include "law/File/Utils.h"
#include "law/RetCode/RetCode.h"

namespace law::file::utils {
    FilePermissionTest FilePermissionTest::operator|(const FilePermissionTest &rhs) const noexcept {
        return {getAsIntegral() | rhs.getAsIntegral()};
    }

    ReadPermissionTest::ReadPermissionTest() : FilePermissionTest(R_OK) {}

    WritePermissionTest::WritePermissionTest() : FilePermissionTest(W_OK) {}

    ExecutePermissionTest::ExecutePermissionTest() : FilePermissionTest(X_OK) {}

    BoolRet fileExist(std::string_view path) noexcept {
        if (access(path.data(), F_OK) == 0) {
            return true;
        }

        if (errno == ENOENT) {
            return false;
        }

        return fromErrno();
    }

    BoolRet testPermission(std::string_view path, const FilePermissionTest &permission) noexcept {
        return ((access(path.data(), permission.getAsIntegral()) == -1) ? fromErrno() : BoolRet(true));
    }
}