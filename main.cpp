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

int convertDrToInt(uint16_t dr) {
    switch(dr) {
        case REGISTER_CONFIG_DR_128_SPS:
            return 128;
        case REGISTER_CONFIG_DR_250_SPS:
            return 250;
        case REGISTER_CONFIG_DR_490_SPS:
            return 490;
        case REGISTER_CONFIG_DR_1600_SPS:
            return 1600;
        case REGISTER_CONFIG_DR_2400_SPS:
            return 2400;
        case REGISTER_CONFIG_DR_3300_SPS:
            return 3300;
        default:
            return 2400; 
    }
}

int main() {
    TLA2024 *tla = new TLA2024();
    tla->init();

    std::cout << std::hex << std::setfill('0') << std::setw(4) << std::uppercase << tla->getMode() << std::nouppercase << std::dec  << std::endl;
    
    uint16_t adc0, adc1, adc2, adc3;

    tla->setFullScaleRange(REGISTER_CONFIG_FSR_1_024V);
    tla->setDataRate(REGISTER_CONFIG_DR_250_SPS);

    do {
        adc0 = tla->readAdc(REGISTER_CONFIG_MUX_0_GND);
        adc1 = tla->readAdc(REGISTER_CONFIG_MUX_1_GND);
        adc2 = tla->readAdc(REGISTER_CONFIG_MUX_2_GND);
        adc3 = tla->readAdc(REGISTER_CONFIG_MUX_3_GND);

        std::cout << std::hex << "adc0: " << adc0 << std::endl;
        std::cout << std::hex << "adc1: " << adc1 << std::endl;
        std::cout << std::hex << "adc2: " << adc2 << std::endl;
        std::cout << std::hex << "adc3: " << adc3 << std::endl;


        adc0 = tla->readAdc(REGISTER_CONFIG_MUX_0_1);
        adc1 = tla->readAdc(REGISTER_CONFIG_MUX_0_3);
        adc2 = tla->readAdc(REGISTER_CONFIG_MUX_1_3);
        adc3 = tla->readAdc(REGISTER_CONFIG_MUX_2_3);

        std::cout << std::hex << "adc0_1: " << adc0 << std::endl;
        std::cout << std::hex << "adc0_3: " << adc1 << std::endl;
        std::cout << std::hex << "adc1_3: " << adc2 << std::endl;
        std::cout << std::hex << "adc2_3: " << adc3 << std::endl;

        sleep(1);
    } while(true);
}
