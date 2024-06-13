#pragma once

#include "core/battery/battery.h"
#include "config/config.h"

#include <iomanip>
#include <chrono>
#include <thread>
#include <iostream>
#include <law/GPIO/SysfsGPIO.h>
#include <elaw/GPIO/SysfsGPIO.h>
#include <Logger/Utils.h>

namespace core {
    namespace bch {
        class BatteryCharger {
            public:
                static constexpr std::string_view kModuleName = "BatteryChargerModule";
            public:
            enum BatteryChargeStatus {
                ERROR = 0x0000,
                IN_CHARGE = 0x0001,
                FULL_CHARGE = 0x0010
            };
            private:
                config::Core configCore;

                law::gpio::SysfsGPIO batteryWarmingControlGpio;
                law::gpio::SysfsGPIO batteryEnablingControlGpio;
                law::gpio::SysfsGPIO batteryChargingControlGpio;
                law::gpio::SysfsGPIO batteryStatusFirstGpio;
                law::gpio::SysfsGPIO batteryStatusSecondGpio;

                elaw::gpio::SysfsGPIO batteryStatusFirstGpioEvent;
                elaw::gpio::SysfsGPIO batteryStatusSecondGpioEvent;

                float temperature;
                float voltage;

                Battery battery;
                BatteryChargeStatus batteryChargeStatus;
            public:
                BatteryCharger(Battery &battery, config::Core configCore_, law::ep::poll::PollObsPtr pollPtr);
                
                void startAutoCharge();
            private:
                void startWarming();
                void endWarming();

                void chargeEnable();
                void chargeDisable();

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