#include "TLA2024.cpp"
#include "TLA2024.h"
#include <iostream>
#include <iomanip>

float convertFsrToInt(uint16_t fsr) {
    switch(fsr) {
        case REGISTER_CONFIG_FSR_0_256V:
            return 0.256f;
        case REGISTER_CONFIG_FSR_0_512V:
            return 0.512f;
        case REGISTER_CONFIG_FSR_1_024V:
            return 1.024f;
        case REGISTER_CONFIG_FSR_2_048V:
            return 2.048f;
        case REGISTER_CONFIG_FSR_4_096V:
            return 4.096f;
        case REGISTER_CONFIG_FSR_6_144V:
            return 6.144f;
        default:
            return 2.048f; 
    }
}

int main() {
    TLA2024 *tla = new TLA2024();
    tla->init();

    std::cout << std::hex << std::setfill('0') << std::setw(4) << std::uppercase << tla->getMode() << std::nouppercase << std::dec  << std::endl;

    
    uint16_t adc0, adc1, adc2, adc3;

    do {
        adc0 = tla->readAdc(REGISTER_CONFIG_MUX_0_GND);
        adc1 = tla->readAdc(REGISTER_CONFIG_MUX_1_GND);
        adc2 = tla->readAdc(REGISTER_CONFIG_MUX_2_GND);
        adc3 = tla->readAdc(REGISTER_CONFIG_MUX_3_GND);

        std::cout << convertFsrToInt(adc0) << std::endl;
        std::cout << convertFsrToInt(adc1) << std::endl;
        std::cout << convertFsrToInt(adc2) << std::endl;
        std::cout << convertFsrToInt(adc3) << std::endl;

        sleep(1);
    } while(true);
}
