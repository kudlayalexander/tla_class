#pragma once

#include "../battery/battery.h"
#include <iomanip>
#include <chrono>
#include <thread>
#include <iostream>
#include "law/GPIO/SysfsGPIO.h"

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

        Battery battery;

        uint16_t channel;
    public:
        BatteryCharger();
        BatteryCharger(const Battery &battery);
        BatteryCharger(const Battery &battery, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j);

        void start(const char* i2c_path = "/dev/i2c-2");

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

        void startWarming();
        void endWarming();
    private:

        void allowCharging();
        void prohibitCharging();

        bool batteryIsPowerSource();
        bool batteryNeedsCharge();
        
        bool isTemperatureInRange(int start, int end);
        bool isTemperatureLower(int threshhold);
        bool isTemperatureHigher(int threshold);

        void enableBattery();
        void disableBattery(); 

        uint8_t getChargingStatus();

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