#include "battery_charger.h"


core::bch::BatteryCharger::BatteryCharger() {
    battery = core::battery::Battery();
}
core::bch::BatteryCharger::BatteryCharger(const core::battery::Battery &battery_) {
    battery = battery_;
}

core::bch::BatteryCharger::BatteryCharger(const core::battery::Battery &battery_, std::chrono::seconds connectAwaitTimeoutSec_, std::chrono::hours heatDurationH_, std::chrono::hours tempRangeRetryTimeoutH_, 
                                float startChargeAtVolts_, std::chrono::hours chargeStatusUpdatePeriodH_, std::chrono::hours actPwrSourceCheckTimeoutH_,
                                float targetBatteryVoltage_, std::int8_t minTemperature_, std::int8_t maxTemperature_) {
    battery = battery_;
    connectAwaitTimeoutSec = connectAwaitTimeoutSec_;
    heatDurationH = heatDurationH_;
    tempRangeRetryTimeoutH = tempRangeRetryTimeoutH_;
    startChargeAtVolts = startChargeAtVolts_;
    chargeStatusUpdatePeriodH = chargeStatusUpdatePeriodH_;
    actPwrSourceCheckTimeoutH = actPwrSourceCheckTimeoutH_;
    targetBatteryVoltage = targetBatteryVoltage_;
    chargeStatusUpdatePeriodH = chargeStatusUpdatePeriodH_;
    minTemperature = minTemperature_;
    maxTemperature = maxTemperature_;
}

void core::bch::BatteryCharger::startAutoCharge(const char* i2cPath) {
    std::cout << "Started program" << std::endl;

    while (true)
    {
        battery.setI2CPath(i2cPath);
        bool isBatteryConnected = battery.isBatteryConnected();

        if(!isBatteryConnected) {
            // std::cout << "Sleep for secs: " << connectAwaitTimeoutSec << std::endl;
            std::this_thread::sleep_for(connectAwaitTimeoutSec);
            isBatteryConnected = battery.isBatteryConnected();
        }
        else {
            enableBattery();
            endWarming();
            prohibitCharging();

            if (batteryIsPowerSource()) {
                // std::cout << "Sleep for hours: " << actPwrSourceCheckTimeoutH << std::endl;
                std::this_thread::sleep_for(actPwrSourceCheckTimeoutH);
                continue;
            }
            else {
                while (batteryNeedsCharge()) {
                    temperature = battery.getTemperature();

                    if (isTemperatureInRange(minTemperature, maxTemperature)) {
                        allowCharging();

                        while (voltage < startChargeAtVolts) {
                            // std::cout << "Sleep for hours: " << chargeStatusUpdatePeriodH << std::endl;
                            std::this_thread::sleep_for(chargeStatusUpdatePeriodH);
                            
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
                        if (isTemperatureLower(minTemperature)) {
                            startWarming();

                            // std::cout << "Sleep for hours: " << heatDurationH << std::endl;
                            std::this_thread::sleep_for(heatDurationH);
                            
                            if (battery.isBatteryConnected()) {
                                voltage = battery.getVoltage();
                                temperature = battery.getTemperature();
                            }
                            else {
                                break;
                            }

                            if (isTemperatureLower(minTemperature)) {
                                // std::cout << "Sleep for hours: " << tempRangeRetryTimeoutH << std::endl;
                                std::this_thread::sleep_for(tempRangeRetryTimeoutH);
                            }

                            endWarming();
                        }
                        else if (isTemperatureHigher(maxTemperature)) {
                            // std::cout << "Sleep for hours: " << tempRangeRetryTimeoutH << std::endl;
                            std::this_thread::sleep_for(tempRangeRetryTimeoutH);
                        }
                    }
                }
                if (battery.isBatteryConnected() && !batteryNeedsCharge()) {
                    // std::cout << "Sleep for hours: " << chargeStatusUpdatePeriodH << std::endl;
                    std::this_thread::sleep_for(chargeStatusUpdatePeriodH);
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

    std::cout << "Current voltage: " << voltage << std::endl;
    std::cout << "Minimum voltage: " << std::fixed << std::setprecision(2) << startChargeAtVolts << std::endl;

    bool result = voltage < startChargeAtVolts;
    std::cout << "Battery needs charge: " << result << std::endl;

    return voltage < startChargeAtVolts;
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
    return temperature > start && temperature < end;
}

bool core::bch::BatteryCharger::isTemperatureLower(float threshold) {
    bool result = temperature < threshold;
    std::cout << "Current temperature: " << std::fixed << std::setprecision(2) << temperature << std::endl;
    std::cout << "Is temperature lower than 0: " << result << std::endl;
    return temperature < threshold;
}

bool core::bch::BatteryCharger::isTemperatureHigher(float threshold) {
    bool result = temperature > threshold;
    std::cout << "Current temperature: " << std::fixed << std::setprecision(2) << temperature << std::endl;
    std::cout << "Is temperature higher than 60: " << result << std::endl;
    return temperature > threshold;
}

void core::bch::BatteryCharger::setConnectAwaitTimeoutSec(std::chrono::seconds connectAwaitTimeoutSec_) {
    connectAwaitTimeoutSec = connectAwaitTimeoutSec_;
}
std::chrono::seconds  core::bch::BatteryCharger::getConnectAwaitTimeoutSec() {
    return connectAwaitTimeoutSec;
}

void core::bch::BatteryCharger::setHeatDurationH(std::chrono::hours heatDurationH_) {
    heatDurationH = heatDurationH_;
}
std::chrono::hours  core::bch::BatteryCharger::getHeatDurationH() {
    return heatDurationH;
}

void core::bch::BatteryCharger::setTempRangeRetryTimeoutH(std::chrono::hours tempRangeRetryTimeoutH_) {
    tempRangeRetryTimeoutH = tempRangeRetryTimeoutH_;
}
std::chrono::hours  core::bch::BatteryCharger::getTempRangeRetryTimeoutH() {
    return tempRangeRetryTimeoutH;
}

void core::bch::BatteryCharger::setStartChargeAtVolts(float startChargeAtVolts_) {
    startChargeAtVolts = startChargeAtVolts_;
}
float core::bch::BatteryCharger::getStartChargeAtVolts() {
    return startChargeAtVolts;
}

void core::bch::BatteryCharger::setChargeStatusUpdatePeriodH(std::chrono::hours chargeStatusUpdatePeriodH_) {
    chargeStatusUpdatePeriodH = chargeStatusUpdatePeriodH_;
}
std::chrono::hours  core::bch::BatteryCharger::getChargeStatusUpdatePeriodH() {
    return chargeStatusUpdatePeriodH;
}

void core::bch::BatteryCharger::setActPwrSourceCheckTimeoutH(std::chrono::hours actPwrSourceCheckTimeoutH_) {
    actPwrSourceCheckTimeoutH = actPwrSourceCheckTimeoutH_;
}
std::chrono::hours  core::bch::BatteryCharger::getActPwrSourceCheckTimeoutH() {
    return actPwrSourceCheckTimeoutH;
}

void core::bch::BatteryCharger::setTargetBatteryVoltage(float targetBatteryVoltage_) {
    targetBatteryVoltage = targetBatteryVoltage_;
}
float core::bch::BatteryCharger::getTargetBatteryVoltage() {
    return targetBatteryVoltage;
}
