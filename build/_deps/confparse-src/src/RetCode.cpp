#include "confparse/RetCode.h"

namespace cp {
    boost::leaf::error_id newError(RetCode rc) {
        return boost::leaf::new_error(rc);
    }
}