#include "battery.h"
#include <iostream>
#include <iomanip>

int test_battery() {
    std::unique_ptr<Battery> battery {std::make_unique<Battery>()};
    battery->connectBattery();
    std::cout << battery->getTemperature() << std::endl;
    std::cout << battery->getVoltage() << std::endl;
    return 0;
}