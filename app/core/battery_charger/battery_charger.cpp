#include "battery_charger.h"

BatteryCharger::BatteryCharger() {
    battery = Battery();
}
BatteryCharger::BatteryCharger(const Battery &battery_) {
    battery = battery_;
}

BatteryCharger::BatteryCharger(const Battery &battery_, int a_, int b_, int c_, float d_, int e_, int f_, int g_, float h_, int i_, int j_) {
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
    std::cout << "Started program" << std::endl;

    while (true)
    {
        battery.setI2CPath(i2cPath);
        bool isBatteryConnected = battery.isBatteryConnected();

        if(!isBatteryConnected) {
            std::cout << "Sleep for secs: " << a << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(a));
            isBatteryConnected = battery.isBatteryConnected();
        }
        else {
            enableBattery();
            endWarming();
            prohibitCharging();

            if (batteryIsPowerSource()) {
                std::cout << "Sleep for minutes: " << g << std::endl;
                std::this_thread::sleep_for(std::chrono::minutes(g));
                continue;
            }
            else {
                while (batteryNeedsCharge()) {
                    temperature = battery.getTemperature();

                    if (isTemperatureInRange(MIN_TEMPERATURE, MAX_TEMPERATURE)) {
                        allowCharging();

                        while (voltage < h) {
                            std::cout << "Sleep for minutes: " << i << std::endl;
                            std::this_thread::sleep_for(std::chrono::minutes(i));
                            
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
                        if (isTemperatureLower(MIN_TEMPERATURE)) {
                            startWarming();

                            std::cout << "Sleep for minutes: " << b << std::endl;
                            std::this_thread::sleep_for(std::chrono::minutes(b));
                            
                            if (battery.isBatteryConnected()) {
                                voltage = battery.getVoltage();
                                temperature = battery.getTemperature();
                            }
                            else {
                                break;
                            }

                            if (isTemperatureLower(MIN_TEMPERATURE)) {
                                std::cout << "Sleep for minutes: " << c << std::endl;
                                std::this_thread::sleep_for(std::chrono::minutes(c));
                            }
                            else {
                                std::cout << "Sleep for minutes: " << f << std::endl;
                                std::this_thread::sleep_for(std::chrono::minutes(f));
                            }
                            endWarming();
                        }
                        else if (isTemperatureHigher(MAX_TEMPERATURE)) {
                            std::cout << "Sleep for minutes: " << j << std::endl;
                            std::this_thread::sleep_for(std::chrono::minutes(j));
                        }
                    }
                }
                if (battery.isBatteryConnected() && !batteryNeedsCharge()) {
                    std::cout << "Sleep for minutes: " << e << std::endl;
                    std::this_thread::sleep_for(std::chrono::minutes(e));
                }
            }
        } 
    }
}

void BatteryCharger::startWarming() {
    std::unique_ptr<law::gpio::SysfsGPIO> gpio {std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO2, 17)};
    gpio->setPinMode(law::gpio::PinMode::OUTPUT);
    gpio->setState(true);
    std::cout << "Warming started" << std::endl;
}

void BatteryCharger::endWarming() {
    std::unique_ptr<law::gpio::SysfsGPIO> gpio {std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO2, 17)};
    gpio->setPinMode(law::gpio::PinMode::OUTPUT);
    gpio->setState(false);

    std::cout << "Warming ended" << std::endl;
}

void BatteryCharger::enableBattery() {
    std::unique_ptr<law::gpio::SysfsGPIO> gpio {std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO2, 18)};
    gpio->setPinMode(law::gpio::PinMode::OUTPUT);
    gpio->setState(true);

    std::cout << "Battery enabled" << std::endl;
}

void BatteryCharger::disableBattery() {
    std::unique_ptr<law::gpio::SysfsGPIO> gpio {std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO2, 18)};
    gpio->setPinMode(law::gpio::PinMode::OUTPUT);
    gpio->setState(false);

    std::cout << "Battery disabled" << std::endl;
} 

void BatteryCharger::allowCharging() {
    std::unique_ptr<law::gpio::SysfsGPIO> gpio {std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO3, 27)};
    gpio->setPinMode(law::gpio::PinMode::OUTPUT);
    gpio->setState(false);

    std::cout << "Charging allowed" << std::endl;
}

void BatteryCharger::prohibitCharging() {
    std::unique_ptr<law::gpio::SysfsGPIO> gpio {std::make_unique<law::gpio::SysfsGPIO>(law::gpio::Port::GPIO3, 27)};
    gpio->setPinMode(law::gpio::PinMode::OUTPUT);
    gpio->setState(true);

    std::cout << "Charging prohibited" << std::endl;
}

bool BatteryCharger::batteryIsPowerSource() {
    return false;
    std::cout << "Battery is NOT power source" << std::endl;
}

bool BatteryCharger::batteryNeedsCharge() {
    voltage = battery.getVoltage();

    std::cout << "Current voltage: " << voltage << std::endl;
    std::cout << "Minimum voltage: " << std::fixed << std::setprecision(2) << d << std::endl;

    bool result = voltage < d;
    std::cout << "Battery needs charge: " << result << std::endl;

    return voltage < d;
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

bool BatteryCharger::isTemperatureInRange(float start, float end) {
    bool result = temperature > start && temperature < end;
    std::cout << "Current temperature: " << std::fixed << std::setprecision(2) << temperature << std::endl;
    std::cout << "Is temperature in range 0, 60: " << result << std::endl;
    return temperature > start && temperature < end;
}
bool BatteryCharger::isTemperatureLower(float threshold) {
    bool result = temperature < threshold;
    std::cout << "Current temperature: " << std::fixed << std::setprecision(2) << temperature << std::endl;
    std::cout << "Is temperature lower than 0: " << result << std::endl;
    return temperature < threshold;
}
bool BatteryCharger::isTemperatureHigher(float threshold) {
    bool result = temperature > threshold;
    std::cout << "Current temperature: " << std::fixed << std::setprecision(2) << temperature << std::endl;
    std::cout << "Is temperature higher than 60: " << result << std::endl;
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

void BatteryCharger::setD(float d_) {
    d = d_;
}
float  BatteryCharger::getD() {
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

void BatteryCharger::setH(float h_) {
    h = h_;
}
float BatteryCharger::getH() {
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
