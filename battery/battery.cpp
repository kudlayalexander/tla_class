#include "battery.h"

Battery::Battery() {
    adc = TLA2024();
    temperature_sensor = Sensor();
    channel = TLA2024::MUX_0_GND;
    i2cPath = I2C_DEFAULT_PATH;
}

Battery::Battery(const TLA2024 &adc_, const Sensor &temperature_sensor_, uint16_t channel_, const char *i2cPath_) {
    adc = adc_;
    temperature_sensor = temperature_sensor_;
    channel = channel_;
    i2cPath = i2cPath_;
};

float Battery::getTemperature() {
    uint16_t voltageInBits = adc.readAdc(channel);
    float voltage = adc.calculateVoltage(voltageInBits);
    temperature_sensor.setVoltage(voltage);
    return temperature_sensor.getTemperature();
}

float Battery::getVoltage() {
    uint16_t voltageInBits = adc.readAdc(channel);
    float voltage = adc.calculateVoltage(voltageInBits);
    return voltage;
}

bool Battery::isBatteryConnected() {
    return adc.connectToSlave(i2cPath);
}

void Battery::setAdc(const TLA2024 &adc_) {
    adc = adc_;
}

void Battery::setSensor(const Sensor &temperature_sensor_) {
    temperature_sensor = temperature_sensor_;
}

void Battery::setChannel(uint16_t channel_) {
    channel = channel_;
}

void Battery::setI2CPath(const char* i2cPath_) {
    i2cPath = i2cPath_;
}

