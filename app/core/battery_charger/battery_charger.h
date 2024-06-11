#pragma once

#include "core/battery/battery.h"
#include "config/config.h"

#include <iomanip>
#include <chrono>
#include <thread>
#include <iostream>
#include <law/GPIO/SysfsGPIO.h>

namespace core {
    namespace bch {
        class BatteryCharger {
            private:
                config::Core configCore;

                float temperature;
                float voltage;

                battery::Battery battery;
            public:
                BatteryCharger();
                BatteryCharger(const battery::Battery &battery);
                BatteryCharger(const battery::Battery &battery, config::Core configCore_);
                
                void startAutoCharge(const char* i2c_path = "/dev/i2c-2");

                std::chrono::seconds getConnectAwaitTimeoutSec();
                std::chrono::hours getHeatDurationH();
                std::chrono::hours getTempRangeRetryTimeoutH();
                float getStartChargeAtVolts();
                std::chrono::hours getChargeStatusUpdatePeriodH();
                std::chrono::hours getActPwrSourceCheckTimeoutH();
                float getTargetBatteryVoltage();
                int8_t getMinTemperature();
                int8_t getMaxTemperature();
            private:
                void startWarming();
                void endWarming();

                void allowCharging();
                void prohibitCharging();

                bool batteryIsPowerSource();
                bool batteryNeedsCharge();
                
                bool isTemperatureInRange(float start, float end);
                bool isTemperatureLower(float threshhold);
                bool isTemperatureHigher(float threshold);

                void enableBattery();
                void disableBattery(); 

                // law::BoolRet BatteryCharger::getChargingStatusFirst();

                // law::BoolRet BatteryCharger::getChargingStatusSecond();

                // law::BoolRet BatteryCharger::getChargingStatus(law::gpio::Port port, std::uint8_t pinNumber);


                typedef enum BatteryChargeStatus {
                    ERROR = 0x00,
                    CHARGING_FINISHED = 0x01,
                    CHARGING_IN_PROGRESS = 0x10
                } uint8_t;

                
        };
    }
}