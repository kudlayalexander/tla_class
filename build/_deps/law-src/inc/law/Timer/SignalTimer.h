#pragma once

#include <ctime>
#include <csignal>

#include "ITimer.h"

namespace law::timer {
    class SignalTimer : public ITimer {
    private:
        itimerspec its{};
        TimerMode timerMode = TimerMode::REPEAT;
        timer_t timerid = nullptr;
        OnTimer onTimerHandler = []() { /* default */ };

    public:
        ~SignalTimer() noexcept override;

        [[nodiscard]] bool isArmed() const noexcept override;

        [[nodiscard]] std::chrono::nanoseconds getPeriod() const noexcept override;

        void setOnTimerHandler(OnTimer &&cb) noexcept override;

        Status setPeriod(std::chrono::nanoseconds period) noexcept override;

        void setMode(TimerMode tm) noexcept override;

        Status start() noexcept override;

        Status stop() noexcept override;

    private:
        friend void timerSignalHandler(int, siginfo_t *info, void *);
    };
}