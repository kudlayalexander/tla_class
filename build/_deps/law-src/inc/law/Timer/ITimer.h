#pragma once

#include <chrono>
#include <functional>

#include <law/Types.h>

#include "TimerMode.h"

namespace law::timer {
    struct ITimer {
    public:
        using OnTimer = std::function<void()>;

    public:
        virtual ~ITimer() noexcept = default;

        [[nodiscard]] virtual bool isArmed() const noexcept = 0;

        [[nodiscard]] virtual std::chrono::nanoseconds getPeriod() const noexcept = 0;

        virtual void setOnTimerHandler(OnTimer &&cb) noexcept = 0;

        virtual Status setPeriod(std::chrono::nanoseconds period) noexcept = 0;

        virtual void setMode(TimerMode tm) noexcept = 0;

        virtual Status start() noexcept = 0;

        virtual Status stop() noexcept = 0;
    };
}