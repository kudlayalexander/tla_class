#include "battery_charger.h"

core::bch::BatteryCharger::BatteryCharger(Battery &battery_, config::Core configCore_, law::ep::poll::PollObsPtr pollPtr):
batteryWarmingControlGpio(law::gpio::Port::GPIO2, 17),
batteryEnablingControlGpio(law::gpio::Port::GPIO2, 18),
batteryChargingControlGpio(law::gpio::Port::GPIO3, 27),
batteryStatusFirstGpio(law::gpio::Port::GPIO3, 7),
batteryStatusSecondGpio(law::gpio::Port::GPIO3, 4),
batteryStatusFirstGpioEvent(law::gpio::Port::GPIO3, 7, pollPtr),
batteryStatusSecondGpioEvent(law::gpio::Port::GPIO3, 4, pollPtr)
{
    battery = battery_;
    configCore = configCore_;
    batteryWarmingControlGpio.setPinMode(law::gpio::PinMode::OUTPUT);
    batteryEnablingControlGpio.setPinMode(law::gpio::PinMode::OUTPUT);
    batteryChargingControlGpio.setPinMode(law::gpio::PinMode::OUTPUT);
    batteryStatusFirstGpio.setPinMode(law::gpio::PinMode::INPUT);
    batteryStatusSecondGpio.setPinMode(law::gpio::PinMode::INPUT);
}

void core::bch::BatteryCharger::startAutoCharge()
{
    LOGGER_INFO(kModuleName, "Charge started");
    while (true)
    {
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
            chargeDisable();

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
                        chargeEnable();
                        batteryStatusFirstGpioEvent.setOnLogicalTrueHandler([this]() { 
                            this->chargeDisable();

                            batteryChargeStatus = BatteryChargeStatus::FULL_CHARGE;
                        });

                        batteryStatusSecondGpioEvent.setOnLogicalTrueHandler([this]() { 
                            batteryChargeStatus = BatteryChargeStatus::IN_CHARGE;
                        });

                        batteryStatusFirstGpioEvent.setOnLogicalFalseHandler([this]() { 
                            batteryChargeStatus = static_cast<BatteryChargeStatus>(batteryChargeStatus & 0x0001);

                            if (batteryChargeStatus == BatteryChargeStatus::ERROR) {
                                this->chargeDisable();
                                LOGGER_ERROR(kModuleName, "Charge status error");
                            }
                        });

                        batteryStatusSecondGpioEvent.setOnLogicalFalseHandler([this]() { 
                            batteryChargeStatus = static_cast<BatteryChargeStatus>(batteryChargeStatus & 0x0010);

                            if (batteryChargeStatus == BatteryChargeStatus::ERROR) {
                                this->chargeDisable();
                                LOGGER_ERROR(kModuleName, "Charge status error");
                            }
                        });
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

void core::bch::BatteryCharger::chargeEnable()
{
    batteryChargingControlGpio.setState(false);
}

void core::bch::BatteryCharger::chargeDisable()
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