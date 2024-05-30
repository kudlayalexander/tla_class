#include "temperature_sensor.h"

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
    // https://colab.research.google.com/drive/1yGktFqW0doqNn0V1L1ZOb1zqJbIHToTf?usp=sharing
    float linearRegressionCoeffs[7];


    
    return calculateValueByWeights(linearRegressionCoeffs, voltage);
};

float TemperatureSensor::calculateResistance() {
    // https://colab.research.google.com/drive/1yGktFqW0doqNn0V1L1ZOb1zqJbIHToTf?usp=sharing
    float linearRegressionCoeffs[11] = {[ 0, -3.48377576e+09f,  9.57536152e+09f, -1.53773476e+10f,
        1.59803002e+10f, -1.12309086e+10f,  5.40719448e+09f, -1.76155319e+09f,
        3.71766254e+08f, -4.59154778e+07f,  2.52123921e+06f]}

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
