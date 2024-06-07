#pragma once

#define BOOST_LEAF_NO_EXCEPTIONS
#define BOOST_LEAF_CFG_STD_STRING 0
#define BOOST_LEAF_CFG_DIAGNOSTICS 0
#define BOOST_LEAF_CFG_STD_SYSTEM_ERROR 0

#include <boost/leaf.hpp>

namespace ErrorCode {
    using Status = boost::leaf::result<void>;

    template <typename T>
    using CustomError = boost::leaf::result<T>;
}
