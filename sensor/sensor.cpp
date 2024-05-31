#include "sensor.h"
#include <iostream>

Sensor::Sensor() {};

Sensor::Sensor(float voltage_) {
    voltage = voltage_;
}

float Sensor::getTemperature() {
    return static_cast<float>(calculateTemperature());
};

float Sensor::getResistance() {
    return calculateResistance();
};

float Sensor::getVoltage() {
    return voltage;
};

void Sensor::setVoltage(float voltage_) {
    voltage = voltage_;
}

float Sensor::calculateTemperature() {
    if (voltage < 0.98) return MAX_TEMPERATURE;
    if (voltage > 2.82) return MIN_TEMPERATURE;

    int coeffs_len = 8;

    // https://colab.research.google.com/drive/1yGktFqW0doqNn0V1L1ZOb1zqJbIHToTf?usp=sharing
    long double temperatureRegressionCoeffs[coeffs_len] = {5434.23211731408810010180,
                                                -20169.46624602146766847000,
                                                32463.93311104805616196245,
                                                -28938.22679970147873973474,
                                                15353.40934702806043787859,
                                                -4841.03771459477320604492,
                                                839.51037588934468658408,
                                                -61.78154250354418763891};

    return calculateValueByWeights(temperatureRegressionCoeffs, coeffs_len, voltage);
};

float Sensor::calculateResistance() {
    if (voltage < 0.98) return MIN_RESISTANCE;
    if (voltage > 2.82) return MAX_RESISTANCE;

    int coeffs_len = 17;

    // https://colab.research.google.com/drive/1yGktFqW0doqNn0V1L1ZOb1zqJbIHToTf?usp=sharing
    long double resistanceRegressionCoeffs[coeffs_len] = {2632885515.13546800613403320312,
                                                -17311600570.38995361328125000000,
                                                47184509415.49579620361328125000,
                                                -63846048453.61897277832031250000,
                                                30425587321.68020629882812500000,
                                                32478530833.11248779296875000000,
                                                -55239679692.87566375732421875000,
                                                12613996299.49818611145019531250,
                                                45302555776.64758300781250000000,
                                                -65979595200.79684448242187500000,
                                                49283512285.00073242187500000000,
                                                -23980911162.89762115478515625000,
                                                8027982169.40781688690185546875,
                                                -1848415946.54422378540039062500,
                                                281176298.88350683450698852539,
                                                -25534498.30731179192662239075,
                                                1050791.21016146545298397541};

    float resistance = calculateValueByWeights(resistanceRegressionCoeffs, coeffs_len, voltage);
    if (resistance < 0) {
        resistance = 0;
    }

    return resistance;
};

float Sensor::calculateValueByWeights(long double *coeffs, int coeffs_len, float value) {
    long double result = 0;
    long double degree = 1;

    for (int i = 0; i < coeffs_len; i++)
    {
        result += degree * coeffs[i];
        degree*=value;
    }
    return static_cast<float>(result);
};
