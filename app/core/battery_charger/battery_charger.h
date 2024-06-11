#pragma once

#include "core/battery/battery.h"
#include "config/config.h"

#include <iomanip>
#include <chrono>
#include <thread>
#include <iostream>
#include <law/GPIO/SysfsGPIO.h>
#include <Logger/Utils.h>

namespace core {
    namespace bch {
        class BatteryCharger {
            public:
                static constexpr std::string_view kModuleName = "BatteryChargerModule";
            private:
                config::Core configCore;

                law::gpio::SysfsGPIO batteryWarmingControlGpio;
                law::gpio::SysfsGPIO batteryEnablingControlGpio;
                law::gpio::SysfsGPIO batteryChargingControlGpio;
                law::gpio::SysfsGPIO batteryStatusFirstGpio;
                law::gpio::SysfsGPIO batteryStatusSecondGpio;

                float temperature;
                float voltage;

                battery::Battery battery;
            public:
                BatteryCharger(const battery::Battery &battery, config::Core configCore_);
                
                void startAutoCharge(std::string_view i2c_path = "/dev/i2c-2");
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
        };
    }
}