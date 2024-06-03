#include <iostream>
#include <iomanip>
#include "TLA2024.h"

int main() {

    std::unique_ptr<TLA2024> tla {std::make_unique<TLA2024>()};
    tla->connectToSlave();
    
    uint16_t adc0, adc1, adc2, adc3, adc0_1, adc0_3, adc1_3, adc2_3;

    
    tla->setFullScaleRange(tla->FSR_2_048V);
    tla->setDataRate(tla->DR_1600_SPS);

    do {
        adc0 = tla->readRaw(tla->MUX_0_GND);
        adc1 = tla->readRaw(tla->MUX_1_GND);
        adc2 = tla->readRaw(tla->MUX_2_GND);
        adc3 = tla->readRaw(tla->MUX_3_GND);

        std::cout << std::hex << "adc0: " << adc0 << std::dec << std::endl;
        std::cout << std::hex << "adc1: " << adc1 << std::dec <<  std::endl;
        std::cout << std::hex << "adc2: " << adc2 << std::dec <<  std::endl;
        std::cout << std::hex << "adc3: " << adc3 << std::dec <<  std::endl;

        std::cout << std::endl;

        std::cout << std::fixed << std::setprecision(3) << tla->calculateVoltage(adc0) << std::endl;
        std::cout << std::fixed <<  std::setprecision(3) << tla->calculateVoltage(adc1) << std::endl;
        std::cout << std::fixed <<  std::setprecision(3) << tla->calculateVoltage(adc2) << std::endl;
        std::cout << std::fixed <<  std::setprecision(3) << tla->calculateVoltage(adc3) << std::endl;

        std::cout << "-----------------------------" << std::endl;
        std::cout << "-----------------------------" << std::endl;

        adc0_1 = tla->readRaw(tla->MUX_0_1);
        adc0_3 = tla->readRaw(tla->MUX_0_3);
        adc1_3 = tla->readRaw(tla->MUX_1_3);
        adc2_3 = tla->readRaw(tla->MUX_2_3);

        std::cout << std::hex << "adc0_1: " << adc0_1 << std::endl;
        std::cout << std::hex << "adc0_3: " << adc0_3 << std::endl;
        std::cout << std::hex << "adc1_3: " << adc1_3 << std::endl;
        std::cout << std::hex << "adc2_3: " << adc2_3 << std::endl;

        std::cout << std::endl;

        std::cout << std::fixed <<  std::setprecision(3) << tla->calculateVoltage(adc0_1) << std::endl;
        std::cout << std::fixed <<  std::setprecision(3) << tla->calculateVoltage(adc0_3) << std::endl;
        std::cout << std::fixed <<  std::setprecision(3) << tla->calculateVoltage(adc1_3) << std::endl;
        std::cout << std::fixed <<  std::setprecision(3) << tla->calculateVoltage(adc2_3) << std::endl;

        std::cout << "-----------------------------" << std::endl;
        std::cout << "-----------------------------" << std::endl;

        sleep(1);
    } while(true);
}
