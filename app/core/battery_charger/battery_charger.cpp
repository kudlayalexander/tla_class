#include "battery_charger.h"

core::bch::BatteryCharger(const battery::Battery &battery, config::Core configCore_)
{
    battery = battery_;
    configCore = configCore_;

    batteryWarmingControlGpio = law::gpio::SysfsGPIO(law::gpio::Port::GPIO2, 17);
    batteryWarmingControlGpio.setPinMode(law::gpio::PinMode::OUTPUT);

    batteryEnablingControlGpio = law::gpio::SysfsGPIO(law::gpio::Port::GPIO2, 18);
    batteryEnablingControlGpio.setPinMode(law::gpio::PinMode::OUTPUT);

    batteryChargingControlGpio = law::gpio::SysfsGPIO(law::gpio::Port::GPIO3, 27);
    batteryChargingControlGpio.setPinMode(law::gpio::PinMode::OUTPUT);

    batteryStatusFirstGpio = law::gpio::SysfsGPIO(law::gpio::Port::GPIO3, 7);
    batteryStatusFirstGpio.setPinMode(law::gpio::PinMode::INPUT);

    batteryStatusSecondGpio = law::gpio::SysfsGPIO(law::gpio::Port::GPIO3, 4);
    batteryStatusSecondGpio.setPinMode(law::gpio::PinMode::INPUT);
}

void core::bch::BatteryCharger::startAutoCharge(std::string_view i2cPath)
{
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
                                std::this_thread::sleep_for(configCore.heat.tempRangeRetryTimeoutH);
                            }

                            endWarming();
                        }
                        else if (isTemperatureHigher(configCore.heat.tempRangeCelsius.max))
                        {
                            std::this_thread::sleep_for(configCore.heat.tempRangeRetryTimeoutH);
                        }
                    }
                }
                if (battery.isBatteryConnected() && !batteryNeedsCharge())
                {
                    std::this_thread::sleep_for(configCore.charge.chargeStatusUpdatePeriodH);
                }
            }
        }
    }
}

void core::bch::BatteryCharger::startWarming()
{
    batteryWarmingControlGpio.setState(true);
}

void core::bch::BatteryCharger::endWarming()
{
    batteryWarmingControlGpio.setState(false);
}

void core::bch::BatteryCharger::enableBattery()
{
    batteryEnablingControlGpio.setState(true);
}

void core::bch::BatteryCharger::disableBattery()
{
    batteryEnablingControlGpio.setState(false);
}

void core::bch::BatteryCharger::allowCharging()
{
    batteryChargingControlGpio.setState(false);
}

void core::bch::BatteryCharger::prohibitCharging()
{
    batteryChargingControlGpio.setState(true);
}

bool core::bch::BatteryCharger::batteryIsPowerSource()
{
    return false;
}

bool core::bch::BatteryCharger::batteryNeedsCharge()
{
    voltage = battery.getVoltage();
    float startChargeAtVolts = configCore.charge.startChargeAtVolts;

    bool result = voltage < startChargeAtVolts;

    return result;
}

bool core::bch::BatteryCharger::isTemperatureInRange(float start, float end)
{
    bool result = temperature > start && temperature < end;
    return result;
}

bool core::bch::BatteryCharger::isTemperatureLower(float threshold)
{
    bool result = temperature < threshold;
    return result;
}

bool core::bch::BatteryCharger::isTemperatureHigher(float threshold)
{
    bool result = temperature > threshold;
    return result;
}