#include "law/RetCode/RetCode.h"

namespace law {
    boost::leaf::error_id fromErrno() noexcept {
        return boost::leaf::new_error(RetCode(errno));
    }
}