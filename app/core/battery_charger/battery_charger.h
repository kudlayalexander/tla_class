#pragma once

#include "../battery/battery.h"
#include <iomanip>
#include <chrono>
#include <thread>
#include <iostream>
#include <law/GPIO/SysfsGPIO.h>

class BatteryCharger {
    private:
        int a;
        int b;
        int c;
        float d;
        int e;
        int f;
        int g;
        float h;
        int i;
        int j;

        float temperature;
        float voltage;

        Battery battery;
    public:
        BatteryCharger();
        BatteryCharger(const Battery &battery);
        BatteryCharger(const Battery &battery, int a_, int b_, int c_, float d_, int e_, int f_, int g_, float h_, int i_, int j_);

        void start(const char* i2c_path = "/dev/i2c-2");

        void setA(int a_);
        int getA();

        void setB(int b_);
        int getB();

        void setC(int c_);
        int getC();

        void setD(float d_);
        float getD();

        void setE(int e_);
        int getE();

        void setF(int f_);
        int getF();

        void setG(int g_);
        int getG();

        void setH(float h_);
        float getH();

        void setI(int i_);
        int getI();

        void setJ(int j_);
        int getJ();

        void startWarming();
        void endWarming();

        void allowCharging();
        void prohibitCharging();
    private:

        bool batteryIsPowerSource();
        bool batteryNeedsCharge();
        
        bool isTemperatureInRange(float start, float end);
        bool isTemperatureLower(float threshhold);
        bool isTemperatureHigher(float threshold);

        void enableBattery();
        void disableBattery(); 

        // law::BoolRet BatteryCharger::getChargingStatusFirst();

        // law::BoolRet BatteryCharger::getChargingStatusSecond();

        // law::BoolRet BatteryCharger::getChargingStatus(law::gpio::Port port, std::uint8_t pinNumber);

        enum TemperatureRestrictions {
            MIN_TEMPERATURE = 0,
            MAX_TEMPERATURE = 60
        };

        typedef enum BatteryChargeStatus {
            ERROR = 0x00,
            CHARGING_FINISHED = 0x01,
            CHARGING_IN_PROGRESS = 0x10
        } uint8_t;

        
};