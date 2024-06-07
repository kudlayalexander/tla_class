#pragma once

#include <ErrorCode/ErrorCode.h>

namespace law {
    namespace detail {
        template <typename T>
        concept IsStaticContainer = requires {
            T::MAX_SIZE;
        };
    }

    template<class T>
    concept StringLike = std::is_convertible_v<T, std::string_view>;

    using Status = ErrorCode::Status;
    using FdRet = ErrorCode::CustomError<int>;
    using BoolRet = ErrorCode::CustomError<bool>;

    using Blob = std::string;
    using BlobRet = ErrorCode::CustomError<Blob>;

    using BlobView = std::string_view;
    using BlobViewRet = ErrorCode::CustomError<BlobView>;

    using AvailableDataSize = ErrorCode::CustomError<int>;

    using ReadCountRet = ErrorCode::CustomError<std::size_t>;
}