#include "battery_charger.h"
BatteryCharger::BatteryCharger(const TLA2024 &battery_, const Sensor &sensor_) {
    battery = battery_;
    sensor = sensor_;
    channel = TLA2024::MUX_0_GND;
}

BatteryCharger::BatteryCharger(const TLA2024 &battery_, const Sensor &sensor_,uint16_t channel_ = TLA2024::MUX_0_GND, int a_, int b_, int c_, int d_, int e_, int f_, int g_, int h_, int i_, int j_) {
    battery = battery_;
    sensor = sensor_;
    channel = channel_;
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
    bool isConnectedToBattery = battery.connectToSlave(i2cPath);

    while(!isConnectedToBattery) {
        std::this_thread::sleep_for(std::chrono::seconds(a));
        isConnectedToBattery = battery.connectToSlave(i2cPath);
    }

    while(batteryIsPowerSource()) {
        std::this_thread::sleep_for(std::chrono::hours(g));
        continue;

        while (batteryNeedsCharge()) {
            if (isTemperatureInRange(MIN_TEMPERATURE, MAX_TEMPERATURE)) {
                allowCharging();

                while (voltage < h) {
                    std::this_thread::sleep_for(std::chrono::hours(j));
                    measureCurrentVoltageAndTemperature();
                }   
            }
            else {
                if (isTemperatureLower(MIN_TEMPERATURE)) {
                    startWarming();
                    std::this_thread::sleep_for(std::chrono::hours(b));
                    measureCurrentVoltageAndTemperature();

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
        
        std::this_thread::sleep_for(std::chrono::hours(e));
        continue;
    } 
}

void BatteryCharger::startWarming() {
    return;
}
void BatteryCharger::endWarming() {
    return;
}

void BatteryCharger::allowCharging() {
    return;
}
void BatteryCharger::prohibitCharging() {
    return;
}

bool BatteryCharger::batteryIsPowerSource() {
    return true;
}

bool BatteryCharger::batteryNeedsCharge() {
    measureCurrentVoltageAndTemperature();
    return voltage < d;
}

bool BatteryCharger::isTemperatureInRange(int start, int end) {
    return temperature > start && temperature < end;
}
bool BatteryCharger::isTemperatureLower(int threshhold) {
    return temperature < threshhold;
}
bool BatteryCharger::isTemperatureHigher(int threshold) {
    return temperature > threshold;
}

void BatteryCharger::measureCurrentVoltageAndTemperature() {
    uint16_t voltageInBits = battery.readAdc(channel);
    voltage = battery.calculateVoltage(voltageInBits);
    sensor.setVoltage(voltage);
    temperature = sensor.getTemperature();
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