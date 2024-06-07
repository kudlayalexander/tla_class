#include <Utils/Hash/CRC32/CRC32.h>

#include "Logger/Utils.h"

namespace Logger {
    MessagePriority getMessagePriorityByName(std::string_view name) {
        switch (Utils::CRC32(name).get()) {
            case COMPILE_TIME_CRC32_STR("EMERGENCY"):
                return MessagePriority::EMERGENCY;
            case COMPILE_TIME_CRC32_STR("ALERT"):
                return MessagePriority::ALERT;
            case COMPILE_TIME_CRC32_STR("CRITICAL"):
                return MessagePriority::CRITICAL;
            case COMPILE_TIME_CRC32_STR("ERROR"):
                return MessagePriority::ERROR;
            case COMPILE_TIME_CRC32_STR("WARNING"):
                return MessagePriority::WARNING;
            case COMPILE_TIME_CRC32_STR("NOTICE"):
                return MessagePriority::NOTICE;
            case COMPILE_TIME_CRC32_STR("INFO"):
                return MessagePriority::INFO;
            case COMPILE_TIME_CRC32_STR("DEBUG"):
                return MessagePriority::DEBUG;
            default:
                throw std::runtime_error("Unknown log level");
        }
    }
}