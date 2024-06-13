#pragma once

#include <array>
#include <chrono>
#include <variant>
#include <sys/un.h>
#include <net/if.h>

#include <etl/string.h>

namespace config {
    namespace tp {
        struct UdsUdpServer {
            using Path = etl::string<sizeof(sockaddr_un::sun_path)>;

            Path path;

            bool operator==(const UdsUdpServer &rhs) const noexcept {
                return (path == rhs.path);
            }
        };
    }

    struct Api {
        struct Tpapi {
            using Ep = std::variant<tp::UdsUdpServer>;

            Ep ep;

            bool operator==(const Tpapi &rhs) const noexcept {
                return (ep == rhs.ep);
            }
        };


        Tpapi tpapi;

        bool operator==(const Api &rhs) const noexcept {
            return (
                    (tpapi == rhs.tpapi)
            );
        }
    };

    struct Core {
        struct Battery {
            using ConnectAwaitTimeoutSec = std::chrono::seconds;
            static constexpr ConnectAwaitTimeoutSec kConnectAwaitTimeoutSecMinValue = std::chrono::seconds(1);
            static constexpr ConnectAwaitTimeoutSec kConnectAwaitTimeoutSecMaxValue = std::chrono::seconds(600);

            using ActPwrSourceCheckTimeoutHours = std::chrono::hours;
            static constexpr ActPwrSourceCheckTimeoutHours kActPwrSourceCheckTimeoutHoursMinValue = std::chrono::hours(1);
            static constexpr ActPwrSourceCheckTimeoutHours kActPwrSourceCheckTimeoutHoursMaxValue = std::chrono::hours(48);
            
            ConnectAwaitTimeoutSec connectAwaitTimeoutSec;
            ActPwrSourceCheckTimeoutHours actPwrSourceCheckTimeoutHours;

            auto operator <=> (const Battery &) const noexcept = default;
        };

        struct Heat {         
            using HeatDurationH = std::chrono::hours;
            static constexpr HeatDurationH kHeatDurationHMinValue = std::chrono::hours(1);
            static constexpr HeatDurationH kHeatDurationHMaxValue = std::chrono::hours(48);

            using TempRangeRetryTimeoutH = std::chrono::hours;
            static constexpr TempRangeRetryTimeoutH kTempRangeRetryTimeoutHMinValue = std::chrono::hours(1);
            static constexpr TempRangeRetryTimeoutH kTempRangeRetryTimeoutHMaxValue = std::chrono::hours(48);

            struct TempRangeCelsius {
                using Min = std::int8_t;
                static constexpr Min kMinMinValue = -125;
                static constexpr Min kMinMaxValue = 125;

                using Max = std::int8_t;
                static constexpr Max kMaxMinValue = -125;
                static constexpr Max kMaxMaxValue = 125;

                Min min;
                Max max;

                auto operator <=>(const TempRangeCelsius &) const noexcept = default;
            };

            HeatDurationH heatDurationH;
            TempRangeRetryTimeoutH tempRangeRetryTimeoutH;
            TempRangeCelsius tempRangeCelsius;

            auto operator <=> (const Heat &) const noexcept = default;
        };

        struct Charge {
            using ChargeStatusUpdatePeriodH = std::chrono::hours;
            static constexpr ChargeStatusUpdatePeriodH kChargeStatusUpdatePeriodHMinValue = std::chrono::hours(1);
            static constexpr ChargeStatusUpdatePeriodH kChargeStatusUpdatePeriodHMaxValue = std::chrono::hours(48);

            using StartChargeAtVolts = float;
            static constexpr StartChargeAtVolts kStartChargeAtVoltsMinValue = 0.0f;
            static constexpr StartChargeAtVolts kStartChargeAtVoltsMaxValue = 16.0f;

            using TargetBatteryVoltage = float;
            static constexpr TargetBatteryVoltage kTargetBatteryVoltageMinValue = 0.0f;
            static constexpr TargetBatteryVoltage kTargetBatteryVoltageMaxValue = 16.0f;

            ChargeStatusUpdatePeriodH chargeStatusUpdatePeriodH;
            StartChargeAtVolts startChargeAtVolts;
            TargetBatteryVoltage targetBatteryVoltage;

            auto operator <=> (const Charge &) const noexcept = default;
        };

        Battery battery;
        Heat heat;
        Charge charge;

        bool operator==(const Core &rhs) const noexcept {
            return (
                    (battery == rhs.battery) &&
                    (heat == rhs.heat) &&
                    (charge == rhs.charge)
            );
        }
    };

    struct Logging {
        using Level = etl::string<10>;
        using LevelAllowValues = std::array<std::string_view, 8>;
        static constexpr LevelAllowValues kLevelAllowValues = {
                "EMERGENCY", "ALERT", "CRITICAL", "ERROR", "WARNING", "NOTICE", "INFO", "DEBUG"
        };

        Level level;

        bool operator==(const Logging &rhs) const noexcept {
            return (level == rhs.level);
        }
    };

    struct Config {
        Api api;
        Core core;
        Logging logging;

        bool operator==(const Config &rhs) const noexcept {
            return (
                    (api == rhs.api) &&
                    (core == rhs.core) &&
                    (logging == rhs.logging)
            );
        }
    };
}