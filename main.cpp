#include "TLA2024.cpp"
#include "TLA2024.h"
#include <iostream>
#include <iomanip>

int main() {
    TLA2024 *tla = new TLA2024(I2C_DEFAULT_PATH, 0x48);

    std::cout << std::hex << std::setfill('0') << std::setw(4) << std::uppercase << tla->getMode() << std::nouppercase << std::dec  << std::endl;

    tla->setConfigurationRegister();
    int16_t conversion_content = tla->readAdc();

    std::cout << std::hex << std::setfill('0') << std::setw(4) << std::uppercase << conversion_content << std::nouppercase << std::dec  << std::endl;
}
