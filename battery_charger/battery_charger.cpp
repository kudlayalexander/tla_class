#include "battery_charger.h"

BatteryCharger::BatteryCharger() {
    battery = Battery();
}
BatteryCharger::BatteryCharger(const Battery &battery_) {
    battery = battery_;
}

BatteryCharger::BatteryCharger(const Battery &battery_, int a_, int b_, int c_, int d_, int e_, int f_, int g_, int h_, int i_, int j_) {
    battery = battery_;
    a = a_;
    b = b_;
    c = c_;
    d = d_;
    e = e_;
    f = f_;
    g = g_;
    h = h_;
    i = i_;
    j = j_;
}

void BatteryCharger::start(const char* i2cPath) {
    while (true)
    {
        battery.setI2CPath(i2cPath);
        bool isBatteryConnected = battery.isBatteryConnected();

        if(!isBatteryConnected) {
            std::this_thread::sleep_for(std::chrono::seconds(a));
            isBatteryConnected = battery.isBatteryConnected();
        }
        else {
            enableBattery();
            endWarming();
            if (batteryIsPowerSource()) {
                std::this_thread::sleep_for(std::chrono::hours(g));
                continue;
            }
            else {
                while (batteryNeedsCharge()) {
                    temperature = battery.getTemperature();

                    if (isTemperatureInRange(MIN_TEMPERATURE, MAX_TEMPERATURE)) {
                        allowCharging();

                        while (voltage < h) {
                            std::this_thread::sleep_for(std::chrono::hours(j));
                            
                            if (battery.isBatteryConnected())
                            {
                                voltage = battery.getVoltage();
                            }
                            else {
                                prohibitCharging();
                                break;
                            }
                        }

                        prohibitCharging();
                    }
                    else {
                        if (isTemperatureLower(MIN_TEMPERATURE)) {
                            startWarming();
                            std::this_thread::sleep_for(std::chrono::hours(b));
                            
                            if (battery.isBatteryConnected()) {
                                voltage = battery.getVoltage();
                                temperature = battery.getTemperature();
                            }
                            else {
                                endWarming();
                                break;
                            }

                            if (isTemperatureLower(MIN_TEMPERATURE)) {
                                std::this_thread::sleep_for(std::chrono::hours(c));
                            }
                            else {
                                std::this_thread::sleep_for(std::chrono::hours(f));
                            }
                            endWarming();
                        }
                        else if (isTemperatureHigher(MAX_TEMPERATURE)) {
                            std::this_thread::sleep_for(std::chrono::hours(j));
                        }
                    }
                }
                if (battery.isBatteryConnected() && !batteryNeedsCharge()) {
                    std::this_thread::sleep_for(std::chrono::hours(e));
                }
            }
        } 
    }
}

void BatteryCharger::startWarming() {
    std::unique_ptr<law::gpio::SysfsGPIO> gpio {std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO2, 17)};
    gpio->setPinMode(law::gpio::PinMode::INPUT);
    gpio->setState(true);
}

void BatteryCharger::endWarming() {
    std::unique_ptr<law::gpio::SysfsGPIO> gpio {std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO2, 17)};
    gpio->setPinMode(law::gpio::PinMode::INPUT);
    gpio->setState(false);
}

void BatteryCharger::enableBattery() {
    std::unique_ptr<law::gpio::SysfsGPIO> gpio {std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO2, 18)};
    gpio->setPinMode(law::gpio::PinMode::INPUT);
    gpio->setState(true);
}

void BatteryCharger::disableBattery() {
    std::unique_ptr<law::gpio::SysfsGPIO> gpio {std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO2, 18)};
    gpio->setPinMode(law::gpio::PinMode::INPUT);
    gpio->setState(false);
} 

void BatteryCharger::allowCharging() {
    std::unique_ptr<law::gpio::SysfsGPIO> gpio {std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO3, 27)};
    gpio->setPinMode(law::gpio::PinMode::INPUT);
    gpio->setState(false);
}

void BatteryCharger::prohibitCharging() {
    std::unique_ptr<law::gpio::SysfsGPIO> gpio {std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO3, 27)};
    gpio->setPinMode(law::gpio::PinMode::INPUT);
    gpio->setState(true);
}

bool BatteryCharger::batteryIsPowerSource() {
    return true;
}

bool BatteryCharger::batteryNeedsCharge() {
    voltage = battery.getVoltage();
    return voltage < d;
}

// uint8_t BatteryCharger::getChargingStatus() {
//     std::unique_ptr<law::gpio::SysfsGPIO> gpio {std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO3, 7)};
//     gpio->setPinMode(law::gpio::PinMode::OUTPUT);
//     uint8_t status_first = static_cast<std::underlying_type_t<BoolRet>>(gpio->get());

//     gpio->setPin(law::gpio::Port::GPIO3, 4);
//     gpio->setPinMode(law::gpio::PinMode::OUTPUT);
//     uint8_t status_second = static_cast<std::underlying_type_t<BoolRet>>(gpio->get());

//     return status_first << 1 | status_second;
// }

bool BatteryCharger::isTemperatureInRange(int start, int end) {
    return temperature > start && temperature < end;
}
bool BatteryCharger::isTemperatureLower(int threshhold) {
    return temperature < threshhold;
}
bool BatteryCharger::isTemperatureHigher(int threshold) {
    return temperature > threshold;
}

void BatteryCharger::setA(int a_) {
    a = a_;
}
int  BatteryCharger::getA() {
    return a;
}

void BatteryCharger::setB(int b_) {
    b = b_;
}
int  BatteryCharger::getB() {
    return b;
}

void BatteryCharger::setC(int c_) {
    c = c_;
}
int  BatteryCharger::getC() {
    return c;
}

void BatteryCharger::setD(int d_) {
    d = d_;
}
int  BatteryCharger::getD() {
    return d;
}

void BatteryCharger::setE(int e_) {
    e = e_;
}
int  BatteryCharger::getE() {
    return e;
}

void BatteryCharger::setF(int f_) {
    f = f_;
}
int  BatteryCharger::getF() {
    return f;
}

void BatteryCharger::setG(int g_) {
    g = g_;
}
int  BatteryCharger::getG() {
    return g;
}

void BatteryCharger::setH(int h_) {
    h = h_;
}
int  BatteryCharger::getH() {
    return h;
}

void BatteryCharger::setI(int i_) {
    i = i_;
}
int  BatteryCharger::getI() {
    return i;
}

void BatteryCharger::setJ(int j_) {
    j = j_;
}
int  BatteryCharger::getJ() {
    return j;
}
