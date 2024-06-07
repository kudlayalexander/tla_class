#pragma once

#include "ClockId.h"
#include "TimerOption.h"
#include "CreateOption.h"

#include "law/Timer/ITimer.h"
#include "law/FileDescriptor/FileDescriptor.h"

namespace law::timer::tfd {
    class TimerFd : public fd::FileDescriptor, public ITimer {
    private:
        ClockId clockId = ClockId::UNSPECIFIED;
        CreateOption createOptions = CreateOption(0);

        TimerOption timerOptions = TimerOption(0);

        TimerMode timerMode = TimerMode::REPEAT;

        std::chrono::nanoseconds period{};
        OnTimer onTimerHandler = []() { /* default */ };

    public:
        TimerFd() noexcept = default;

        explicit TimerFd(ClockId cid, const CreateOption &co = CreateOption(0));

        Status create(ClockId cid, const CreateOption &co = CreateOption(0)) noexcept;

        [[nodiscard]] bool isArmed() const noexcept override;

        [[nodiscard]] ClockId getClockId() const noexcept;

        [[nodiscard]] CreateOption getCreateOptions() const noexcept;

        [[nodiscard]] TimerOption getTimerOptions() const noexcept;

        [[nodiscard]] std::chrono::nanoseconds getPeriod() const noexcept override;

        void setTimerOptions(TimerOption to) noexcept;

        void setOnTimerHandler(OnTimer &&cb) noexcept override;

        Status setPeriod(std::chrono::nanoseconds period) noexcept override;

        void setMode(TimerMode tm) noexcept override;

        Status start() noexcept override;

        Status stop() noexcept override;

        Status poll() noexcept;
    };
}