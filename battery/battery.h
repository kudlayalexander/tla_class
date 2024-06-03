#include "../tla2024_adc1015/TLA2024.h"
#include "../sensor/sensor.h"

class Battery {
    private:
        TLA2024 adc;
        Sensor temperature_sensor;
        uint16_t channel;
        const char* i2cPath;
    public:
        Battery();
        Battery(const TLA2024 &adc, const Sensor &temperature_sensor, uint16_t channel, const char* i2cPath = I2C_DEFAULT_PATH);
        float getTemperature();
        float getVoltage();
        bool isBatteryConnected();

        void setAdc(const TLA2024 &adc);
        void setSensor(const Sensor &temperature_sensor);
        void setChannel(uint16_t channel);
        void setI2CPath(const char* i2cPath);
};