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

                const law::gpio::SysfsGPIO batteryWarmingGpio {law::gpio::Port::GPIO2, 17};
                const law::gpio::SysfsGPIO batteryChargingGpio {law::gpio::Port::GPIO2, 27};
                const law::gpio::SysfsGPIO batteryEnablingGpio {law::gpio::Port::GPIO2, 18};

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