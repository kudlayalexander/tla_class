#pragma once

#include "law/FlagValue.h"

namespace law::file {
    struct Permission : public UInt32FlagValue {
        using UInt32FlagValue::UInt32FlagValue;

        Permission operator&(const Permission &of) const noexcept;

        Permission operator|(const Permission &of) const noexcept;
    };

    struct OwnerReadWriteExec : Permission {
        OwnerReadWriteExec() noexcept;
    };

    struct OwnerRead : Permission {
        OwnerRead() noexcept;
    };

    struct OwnerWrite : Permission {
        OwnerWrite() noexcept;
    };

    struct OwnerExec : Permission {
        OwnerExec() noexcept;
    };

    struct GroupReadWriteExec : Permission {
        GroupReadWriteExec() noexcept;
    };

    struct GroupRead : Permission {
        GroupRead() noexcept;
    };

    struct GroupWrite : Permission {
        GroupWrite() noexcept;
    };

    struct GroupExec : Permission {
        GroupExec() noexcept;
    };

    struct OtherReadWriteExec : Permission {
        OtherReadWriteExec() noexcept;
    };

    struct OtherRead : Permission {
        OtherRead() noexcept;
    };

    struct OtherWrite : Permission {
        OtherWrite() noexcept;
    };

    struct OtherExec : Permission {
        OtherExec() noexcept;
    };
}