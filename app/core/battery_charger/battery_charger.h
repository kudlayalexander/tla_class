#pragma once

#include "core/battery/battery.h"
#include <iomanip>
#include <chrono>
#include <thread>
#include <iostream>
#include <law/GPIO/SysfsGPIO.h>
#include "config/config.h"

namespace core {
    namespace bch {
        class BatteryCharger {
            private:
                std::chrono::seconds connectAwaitTimeoutSec;
                std::chrono::hours heatDurationH;
                std::chrono::hours tempRangeRetryTimeoutH;
                float startChargeAtVolts;
                std::chrono::hours chargeStatusUpdatePeriodH;
                std::chrono::hours actPwrSourceCheckTimeoutH;
                float targetBatteryVoltage;

                float temperature;
                float voltage;

                int8_t minTemperature;
                int8_t maxTemperature;

                battery::Battery battery;
            public:
                BatteryCharger();
                BatteryCharger(const battery::Battery &battery);
                BatteryCharger(const battery::Battery &battery, std::chrono::seconds connectAwaitTimeoutSec_, std::chrono::hours heatDurationH_, std::chrono::hours tempRangeRetryTimeoutH_, 
                                float startChargeAtVolts_, std::chrono::hours chargeStatusUpdatePeriodH_, std::chrono::hours actPwrSourceCheckTimeoutH_, 
                                float targetBatteryVoltage_);
                
                void startAutoCharge(const char* i2c_path = "/dev/i2c-2");

                void make(config::Core conf);

                void setConnectAwaitTimeoutSec(std::chrono::seconds connectAwaitTimeoutSec_);
                std::chrono::seconds getConnectAwaitTimeoutSec();

                void setHeatDurationH(std::chrono::hours heatDurationH_);
                std::chrono::hours getHeatDurationH();

                void setTempRangeRetryTimeoutH(std::chrono::hours tempRangeRetryTimeoutH_);
                std::chrono::hours getTempRangeRetryTimeoutH();

                void setStartChargeAtVolts(float startChargeAtVolts_);
                float getStartChargeAtVolts();

                void setChargeStatusUpdatePeriodH(std::chrono::hours chargeStatusUpdatePeriodH_);
                std::chrono::hours getChargeStatusUpdatePeriodH();

                void setActPwrSourceCheckTimeoutH(std::chrono::hours actPwrSourceCheckTimeoutH_);
                std::chrono::hours getActPwrSourceCheckTimeoutH();

                void setTargetBatteryVoltage(float targetBatteryVoltage_);
                float getTargetBatteryVoltage();
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