#include "battery_charger.h"

core::bch::BatteryCharger(const battery::Battery &battery, config::Core configCore_)
{
    battery = battery_;
    configCore = configCore_;

    batteryWarmingGpio(law::gpio::Port::GPIO2, 17);
    batteryWarmingGpio.setPinMode(law::gpio::PinMode::OUTPUT);

    batteryChargingGpio(law::gpio::Port::GPIO3, 27);
    batteryChargingGpio.setPinMode(law::gpio::PinMode::OUTPUT);

    batteryEnablingGpio(law::gpio::Port::GPIO2, 18);
    batteryEnablingGpio.setPinMode(law::gpio::PinMode::OUTPUT);
}

void core::bch::BatteryCharger::startAutoCharge(std::string_view i2cPath)
{
    // std::cout << "Started program" << std::endl;

    while (true)
    {
        battery.setI2CPath(i2cPath);
        bool isBatteryConnected = battery.isBatteryConnected();

        if (!isBatteryConnected)
        {
            std::this_thread::sleep_for(configCore.battery.connectAwaitTimeoutSec);
            isBatteryConnected = battery.isBatteryConnected();
        }
        else
        {
            enableBattery();
            endWarming();
            prohibitCharging();

            if (batteryIsPowerSource())
            {
                std::this_thread::sleep_for(configCore.battery.actPwrSourceCheckTimeoutHours);
                continue;
            }
            else
            {
                while (batteryNeedsCharge())
                {
                    temperature = battery.getTemperature();

                    if (isTemperatureInRange(configCore.heat.tempRangeCelsius.min, configCore.heat.tempRangeCelsius.max))
                    {
                        allowCharging();

                        while (voltage < configCore.charge.targetBatteryVoltage)
                        {
                            std::this_thread::sleep_for(configCore.charge.chargeStatusUpdatePeriodH);

                            if (battery.isBatteryConnected())
                            {
                                voltage = battery.getVoltage();
                                std::cout << "Current temperature: " << std::fixed << std::setprecision(2) << battery.getTemperature() << std::endl;
                                std::cout << "Current voltage: " << std::fixed << std::setprecision(2) << voltage << std::endl;
                            }
                            else
                            {
                                break;
                            }
                        }

                        prohibitCharging();
                    }
                    else
                    {
                        if (isTemperatureLower(configCore.heat.tempRangeCelsius.min))
                        {
                            startWarming();

                            // std::cout << "Sleep for hours: " << heatDurationH << std::endl;
                            std::this_thread::sleep_for(configCore.heat.heatDurationH);

                            if (battery.isBatteryConnected())
                            {
                                voltage = battery.getVoltage();
                                temperature = battery.getTemperature();
                            }
                            else
                            {
                                break;
                            }

                            if (isTemperatureLower(configCore.heat.tempRangeCelsius.min))
                            {
                                // std::cout << "Sleep for hours: " << tempRangeRetryTimeoutH << std::endl;
                                std::this_thread::sleep_for(configCore.heat.tempRangeRetryTimeoutH);
                            }

                            endWarming();
                        }
                        else if (isTemperatureHigher(configCore.heat.tempRangeCelsius.max))
                        {
                            // std::cout << "Sleep for hours: " << tempRangeRetryTimeoutH << std::endl;
                            std::this_thread::sleep_for(configCore.heat.tempRangeRetryTimeoutH);
                        }
                    }
                }
                if (battery.isBatteryConnected() && !batteryNeedsCharge())
                {
                    // std::cout << "Sleep for hours: " << chargeStatusUpdatePeriodH << std::endl;
                    std::this_thread::sleep_for(configCore.charge.chargeStatusUpdatePeriodH);
                }
            }
        }
    }
}

void core::bch::BatteryCharger::startWarming()
{
    std::unique_ptr<law::gpio::SysfsGPIO> gpio{std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO2, 17)};
    gpio->setPinMode(law::gpio::PinMode::OUTPUT);
    gpio->setState(true);
    std::cout << "Warming started" << std::endl;
}

void core::bch::BatteryCharger::endWarming()
{
    std::unique_ptr<law::gpio::SysfsGPIO> gpio{std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO2, 17)};
    gpio->setPinMode(law::gpio::PinMode::OUTPUT);
    gpio->setState(false);

    std::cout << "Warming ended" << std::endl;
}

void core::bch::BatteryCharger::enableBattery()
{
    std::unique_ptr<law::gpio::SysfsGPIO> gpio{std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO2, 18)};
    gpio->setPinMode(law::gpio::PinMode::OUTPUT);
    gpio->setState(true);

    std::cout << "Battery enabled" << std::endl;
}

void core::bch::BatteryCharger::disableBattery()
{
    std::unique_ptr<law::gpio::SysfsGPIO> gpio{std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO2, 18)};
    gpio->setPinMode(law::gpio::PinMode::OUTPUT);
    gpio->setState(false);

    std::cout << "Battery disabled" << std::endl;
}

void core::bch::BatteryCharger::allowCharging()
{
    std::unique_ptr<law::gpio::SysfsGPIO> gpio{std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO3, 27)};
    gpio->setPinMode(law::gpio::PinMode::OUTPUT);
    gpio->setState(false);

    std::cout << "Charging allowed" << std::endl;
}

void core::bch::BatteryCharger::prohibitCharging()
{
    std::unique_ptr<law::gpio::SysfsGPIO> gpio{std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO3, 27)};
    gpio->setPinMode(law::gpio::PinMode::OUTPUT);
    gpio->setState(true);

    std::cout << "Charging prohibited" << std::endl;
}

bool core::bch::BatteryCharger::batteryIsPowerSource()
{
    return false;
    std::cout << "Battery is NOT power source" << std::endl;
}

bool core::bch::BatteryCharger::batteryNeedsCharge()
{
    voltage = battery.getVoltage();
    float startChargeAtVolts = configCore.charge.startChargeAtVolts;

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

bool core::bch::BatteryCharger::isTemperatureInRange(float start, float end)
{
    bool result = temperature > start && temperature < end;
    std::cout << "Current temperature: " << std::fixed << std::setprecision(2) << temperature << std::endl;
    std::cout << "Is temperature in range 0, 60: " << result << std::endl;
    return result;
}

bool core::bch::BatteryCharger::isTemperatureLower(float threshold)
{
    bool result = temperature < threshold;
    std::cout << "Current temperature: " << std::fixed << std::setprecision(2) << temperature << std::endl;
    std::cout << "Is temperature lower than 0: " << result << std::endl;
    return result;
}

bool core::bch::BatteryCharger::isTemperatureHigher(float threshold)
{
    bool result = temperature > threshold;
    std::cout << "Current temperature: " << std::fixed << std::setprecision(2) << temperature << std::endl;
    std::cout << "Is temperature higher than 60: " << result << std::endl;
    return result;
}