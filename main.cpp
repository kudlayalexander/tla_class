#include <iostream>
#include <iomanip>
#include "battery/battery.h"
#include <chrono>
#include <thread>

void test() {
    std::unique_ptr<Battery> battery {std::make_unique<Battery>()};
    battery->connectBattery();
    std::cout << battery->getTemperature() << std::endl;
    std::cout << battery->getVoltage() << std::endl;
}

int main() {
    int i = 0;
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        test();
        i++;
    }
}
