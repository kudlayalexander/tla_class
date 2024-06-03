#include "../sensor/sensor.h"
#include "../tla2024_adc1015/TLA2024.h"
#include <iomanip>
#include <chrono>
#include <thread>
#include <iostream>

class BatteryCharger {
    private:
        int a;
        int b;
        int c;
        int d;
        int e;
        int f;
        int g;
        int h;
        int i;
        int j;

        float temperature;
        float voltage;

        TLA2024 battery;
        Sensor sensor;

        uint16_t channel;
    public:
        BatteryCharger(const TLA2024 &battery, const Sensor &sensor);
        BatteryCharger(const TLA2024 &battery, const Sensor &sensor, uint16_t channel = TLA2024::MUX_0_GND, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j);

        void start(const char* i2c_path);

        void setA(int a_);
        int getA();

        void setB(int b_);
        int getB();

        void setC(int c_);
        int getC();

        void setD(int d_);
        int getD();

        void setE(int e_);
        int getE();

        void setF(int f_);
        int getF();

        void setG(int g_);
        int getG();

        void setH(int h_);
        int getH();

        void setI(int i_);
        int getI();

        void setJ(int j_);
        int getJ();
    private:
        void startWarming();
        void endWarming();

        void allowCharging();
        void prohibitCharging();

        bool batteryIsPowerSource();
        bool batteryNeedsCharge();
        
        bool isTemperatureInRange(int start, int end);
        bool isTemperatureLower(int threshhold);
        bool isTemperatureHigher(int threshold);

        void measureCurrentVoltageAndTemperature();

        enum TemperatureRestrictions {
            MIN_TEMPERATURE = 0,
            MAX_TEMPERATURE = 60
        };
};