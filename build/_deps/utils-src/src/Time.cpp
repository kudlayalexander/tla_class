#include "Utils/Time/Time.h"

namespace Utils {
    std::chrono::milliseconds getCurrentTimeInMs() {
        using namespace std::chrono;
        return duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    }
}