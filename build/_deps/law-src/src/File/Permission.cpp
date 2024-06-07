#include <fcntl.h>

#include "law/File/Permission.h"

namespace law::file {
    Permission Permission::operator&(const Permission &of) const noexcept {
        return {getAsIntegral() & of.getAsIntegral()};
    }

    Permission Permission::operator|(const Permission &of) const noexcept {
        return {getAsIntegral() | of.getAsIntegral()};
    }

    OwnerReadWriteExec::OwnerReadWriteExec() noexcept: Permission(S_IRWXU) {}

    OwnerRead::OwnerRead() noexcept: Permission(S_IRUSR) {}

    OwnerWrite::OwnerWrite() noexcept: Permission(S_IWUSR) {}

    OwnerExec::OwnerExec() noexcept: Permission(S_IXUSR) {}

    GroupReadWriteExec::GroupReadWriteExec() noexcept: Permission(S_IRWXG) {}

    GroupRead::GroupRead() noexcept: Permission(S_IRGRP) {}

    GroupWrite::GroupWrite() noexcept: Permission(S_IWGRP) {}

    GroupExec::GroupExec() noexcept: Permission(S_IXGRP) {}

    OtherReadWriteExec::OtherReadWriteExec() noexcept: Permission(S_IRWXO) {}

    OtherRead::OtherRead() noexcept: Permission(S_IROTH) {}

    OtherWrite::OtherWrite() noexcept: Permission(S_IWOTH) {}

    OtherExec::OtherExec() noexcept: Permission(S_IXOTH) {}
}