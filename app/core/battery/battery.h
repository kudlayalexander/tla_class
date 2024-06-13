#pragma once

#include "core/tla2024/TLA2024.h"

namespace core{
    class Battery {
        private:
            TLA2024 adc;
        public:
            Battery();
            Battery(const TLA2024 &adc);

            constexpr float calculateVoltageDivisionScaleFactor(float r1, float r2) {
                return (r1 + r2) / r2;
            }

            float getTemperature();
            float getVoltage();
            bool isBatteryConnected();

            void setAdc(const TLA2024 &adc);

            void connectBattery();
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
}