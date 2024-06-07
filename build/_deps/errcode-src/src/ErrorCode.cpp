#include <stdexcept>

#include "ErrorCode/ErrorCode.h"

namespace boost {
    void throw_exception(const std::exception &e) {
        throw e;
    }
}