class TemperatureSensor {
    private:
        float voltage;
    public:
        float getTemperature();
        float getResistance();
        float getVoltage();
    private:
        float calculateTemperature();
        float calculateResistance();
};