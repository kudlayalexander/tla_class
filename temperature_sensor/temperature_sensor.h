class TemperatureSensor {
    private:
        float voltage;
    public:
        TemperatureSensor();
        TemperatureSensor(float voltage_);
        float getTemperature();
        float getResistance();
        float getVoltage();
        void setVoltage(float voltage_);
    private:
        float calculateTemperature();
        float calculateResistance();
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