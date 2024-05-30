#include "temperature-sensor.h"

float TemperatureSensor::getTemperature() {
    return calculateTemperature();
};

float TemperatureSensor::getResistance() {
    return calculateResistance();
};

float TemperatureSensor::getVoltage() {
    return voltage;
};

float TemperatureSensor::calculateTemperature() {
    float linearRegressionCoeffs[7];

    return calculateValueByWeights(linearRegressionCoeffs, voltage);
};

float TemperatureSensor::calculateResistance() {
    float linearRegressionCoeffs[11];

    return calculateValueByWeights(linearRegressionCoeffs, voltage);
};

float calculateValueByWeights(float *coeffs, float value) {
    float result;
    float degree = 1;
    for (int i = 0; i < sizeof(coeffs); i++)
    {
        result += degree * coeffs[i];
        degree*=value;
    }
    return result;
};
