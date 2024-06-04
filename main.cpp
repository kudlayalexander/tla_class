#include <iostream>
#include <iomanip>
#include "battery_charger/battery_charger.h"
#include <chrono>
#include <thread>

void test() {
    std::unique_ptr<Battery> battery {std::make_unique<Battery>()};
    battery->connectBattery();
    std::cout << battery->getTemperature() << std::endl;
    std::cout << battery->getVoltage() << std::endl;
}

int main() {
    std::unique_ptr<BatteryCharger> batteryCharger {std::make_unique<BatteryCharger>()};
    batteryCharger->startWarming();

    for (int i = 0; i < 60; i++)
    {
        std::cout << "Test " << i << std::endl;
        test();
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    batteryCharger->endWarming();
    std::cout << "Ended warming" << std::endl;

    for (int i = 0; i < 60; i++)
    {
        std::cout << "Test " << i << std::endl;
        test();
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}
