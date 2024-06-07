#pragma once

#include "Logger.h"

namespace Logger {
    MessagePriority getMessagePriorityByName(std::string_view name);
}