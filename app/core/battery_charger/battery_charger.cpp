#include "battery_charger.h"


core::bch::BatteryCharger::BatteryCharger() {
    battery = core::battery::Battery();
}

core::bch::BatteryCharger::BatteryCharger(const core::battery::Battery &battery_) {
    battery = battery_;
}

BatteryCharger(const battery::Battery &battery, config::Core configCore_) {
    battery = battery_;
    configCore = configCore_;
}

void core::bch::BatteryCharger::startAutoCharge(const char* i2cPath) {
    std::cout << "Started program" << std::endl;

    while (true)
    {
        battery.setI2CPath(i2cPath);
        bool isBatteryConnected = battery.isBatteryConnected();

        if(!isBatteryConnected) {
            std::this_thread::sleep_for(getConnectAwaitTimeoutSec());
            isBatteryConnected = battery.isBatteryConnected();
        }
        else {
            enableBattery();
            endWarming();
            prohibitCharging();

            if (batteryIsPowerSource()) {
                std::this_thread::sleep_for(getActPwrSourceCheckTimeoutH());
                continue;
            }
            else {
                while (batteryNeedsCharge()) {
                    temperature = battery.getTemperature();

                    if (isTemperatureInRange(getMinTemperature(), getMaxTemperature())) {
                        allowCharging();

                        while (voltage < getStartChargeAtVolts()) {
                            std::this_thread::sleep_for(getChargeStatusUpdatePeriodH());
                            
                            if (battery.isBatteryConnected())
                            {
                                voltage = battery.getVoltage();
                                std::cout << "Current temperature: " << std::fixed << std::setprecision(2) << battery.getTemperature() << std::endl;
                                std::cout << "Current voltage: " << std::fixed << std::setprecision(2) << voltage << std::endl;
                            }
                            else {
                                break;
                            }
                        }

                        prohibitCharging();
                    }
                    else {
                        if (isTemperatureLower(getMinTemperature())) {
                            startWarming();

                            // std::cout << "Sleep for hours: " << heatDurationH << std::endl;
                            std::this_thread::sleep_for(getHeatDurationH());
                            
                            if (battery.isBatteryConnected()) {
                                voltage = battery.getVoltage();
                                temperature = battery.getTemperature();
                            }
                            else {
                                break;
                            }

                            if (isTemperatureLower(getMinTemperature())) {
                                // std::cout << "Sleep for hours: " << tempRangeRetryTimeoutH << std::endl;
                                std::this_thread::sleep_for(getTempRangeRetryTimeoutH());
                            }

                            endWarming();
                        }
                        else if (isTemperatureHigher(getMaxTemperature())) {
                            // std::cout << "Sleep for hours: " << tempRangeRetryTimeoutH << std::endl;
                            std::this_thread::sleep_for(getTempRangeRetryTimeoutH());
                        }
                    }
                }
                if (battery.isBatteryConnected() && !batteryNeedsCharge()) {
                    // std::cout << "Sleep for hours: " << chargeStatusUpdatePeriodH << std::endl;
                    std::this_thread::sleep_for(getChargeStatusUpdatePeriodH());
                }
            }
        } 
    }
}

void core::bch::BatteryCharger::startWarming() {
    std::unique_ptr<law::gpio::SysfsGPIO> gpio {std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO2, 17)};
    gpio->setPinMode(law::gpio::PinMode::OUTPUT);
    gpio->setState(true);
    std::cout << "Warming started" << std::endl;
}

void core::bch::BatteryCharger::endWarming() {
    std::unique_ptr<law::gpio::SysfsGPIO> gpio {std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO2, 17)};
    gpio->setPinMode(law::gpio::PinMode::OUTPUT);
    gpio->setState(false);

    std::cout << "Warming ended" << std::endl;
}

void core::bch::BatteryCharger::enableBattery() {
    std::unique_ptr<law::gpio::SysfsGPIO> gpio {std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO2, 18)};
    gpio->setPinMode(law::gpio::PinMode::OUTPUT);
    gpio->setState(true);

    std::cout << "Battery enabled" << std::endl;
}

void core::bch::BatteryCharger::disableBattery() {
    std::unique_ptr<law::gpio::SysfsGPIO> gpio {std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO2, 18)};
    gpio->setPinMode(law::gpio::PinMode::OUTPUT);
    gpio->setState(false);

    std::cout << "Battery disabled" << std::endl;
} 

void core::bch::BatteryCharger::allowCharging() {
    std::unique_ptr<law::gpio::SysfsGPIO> gpio {std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO3, 27)};
    gpio->setPinMode(law::gpio::PinMode::OUTPUT);
    gpio->setState(false);

    std::cout << "Charging allowed" << std::endl;
}

void core::bch::BatteryCharger::prohibitCharging() {
    std::unique_ptr<law::gpio::SysfsGPIO> gpio {std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO3, 27)};
    gpio->setPinMode(law::gpio::PinMode::OUTPUT);
    gpio->setState(true);

    std::cout << "Charging prohibited" << std::endl;
}

bool core::bch::BatteryCharger::batteryIsPowerSource() {
    return false;
    std::cout << "Battery is NOT power source" << std::endl;
}

bool core::bch::BatteryCharger::batteryNeedsCharge() {
    voltage = battery.getVoltage();
    float startChargeAtVolts = getStartChargeAtVolts();

    std::cout << "Current voltage: " << voltage << std::endl;
    std::cout << "Minimum voltage: " << std::fixed << std::setprecision(2) << startChargeAtVolts << std::endl;

    bool result = voltage < startChargeAtVolts;
    std::cout << "Battery needs charge: " << result << std::endl;

    return result;
}

// law::BoolRet BatteryCharger::getChargingStatusFirst() {
//     return getChargingStatus(law::gpio::Port::GPIO3, 7);
// }

// law::BoolRet BatteryCharger::getChargingStatusSecond() {
//     return getChargingStatus(law::gpio::Port::GPIO3, 4);
// }

// law::BoolRet BatteryCharger::getChargingStatus(law::gpio::Port port, std::uint8_t pinNumber) {
//     std::unique_ptr<law::gpio::SysfsGPIO> gpio {std::make_unique<law::gpio::SysfsGPIO>(port, pinNumber)};
//     gpio->setPinMode(law::gpio::PinMode::INPUT);
//     return gpio->get();
// }

bool core::bch::BatteryCharger::isTemperatureInRange(float start, float end) {
    bool result = temperature > start && temperature < end;
    std::cout << "Current temperature: " << std::fixed << std::setprecision(2) << temperature << std::endl;
    std::cout << "Is temperature in range 0, 60: " << result << std::endl;
    return result;
}

bool core::bch::BatteryCharger::isTemperatureLower(float threshold) {
    bool result = temperature < threshold;
    std::cout << "Current temperature: " << std::fixed << std::setprecision(2) << temperature << std::endl;
    std::cout << "Is temperature lower than 0: " << result << std::endl;
    return result;
}

bool core::bch::BatteryCharger::isTemperatureHigher(float threshold) {
    bool result = temperature > threshold;
    std::cout << "Current temperature: " << std::fixed << std::setprecision(2) << temperature << std::endl;
    std::cout << "Is temperature higher than 60: " << result << std::endl;
    return result;
}


std::chrono::seconds  core::bch::BatteryCharger::getConnectAwaitTimeoutSec() {
    return configCore.battery.connectAwaitTimeoutSec;
}


std::chrono::hours  core::bch::BatteryCharger::getHeatDurationH() {
    return configCore.heat.heatDurationH;
}


std::chrono::hours  core::bch::BatteryCharger::getTempRangeRetryTimeoutH() {
    return configCore.heat.tempRangeRetryTimeoutH;
}


float core::bch::BatteryCharger::getStartChargeAtVolts() {
    return configCore.charge.startChargeAtVolts;
}


std::chrono::hours  core::bch::BatteryCharger::getChargeStatusUpdatePeriodH() {
    return configCore.charge.chargeStatusUpdatePeriodH;
}


std::chrono::hours  core::bch::BatteryCharger::getActPwrSourceCheckTimeoutH() {
    return configCore.battery.actPwrSourceCheckTimeoutHours;
}


float core::bch::BatteryCharger::getTargetBatteryVoltage() {
    return configCore.charge.targetBatteryVoltage;
}

int8_t core::bch::BatteryCharger::getMinTemperature() {
    return configCore.heat.tempRangeCelsius.min;
}

int8_t core::bch::BatteryCharger::getMaxTemperature() {
    return configCore.heat.tempRangeCelsius.max;
}