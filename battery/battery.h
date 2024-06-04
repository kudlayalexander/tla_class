#pragma once

#include "../tla2024/TLA2024.h"

class Battery {
    private:
        TLA2024 adc;
        const char* i2cPath;
    public:
        Battery();
        Battery(const TLA2024 &adc, const char* i2cPath = I2C_DEFAULT_PATH);

        float getTemperature();
        float getVoltage();
        bool isBatteryConnected();

        void setAdc(const TLA2024 &adc);
        void setI2CPath(const char* i2cPath);

        void connectBattery();
        void connectBattery(const char* i2cPath);
    private:
        float calculateTemperature(float voltage);
        float calculateResistance(float voltage);
        float calculateValueByWeights(long double *coeffs, int coeffs_len, float value);

        enum Temperature {
            MAX_TEMPERATURE = 85,
            MIN_TEMPERATURE = -40
        };

        enum Resistance {
            MAX_RESISTANCE = 403567,
            MIN_RESISTANCE = 1084
        };
};