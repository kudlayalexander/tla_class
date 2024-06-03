#include <iostream>
#include <iomanip>
#include "tla2024_adc1015/TLA2024.h"
#include "sensor/sensor.h"

void test(uint16_t channel) {
    std::unique_ptr<TLA2024> tla {std::make_unique<TLA2024>()};
    tla->connectToSlave();
    tla->setFullScaleRange(tla->FSR_4_096V);
    tla->setDataRate(tla->DR_1600_SPS);

    std::unique_ptr<Sensor> sensor {std::make_unique<Sensor>()};
    
    uint16_t voltageInBits = tla->readRaw(channel);
    channel >>=12;
    float voltage = tla->calculateVoltage(voltageInBits);
    sensor->setVoltage(voltage);
    float temperature = sensor->getTemperature();
    float resistance = sensor->getResistance();

    std::cout << "Voltage on channel " << channel << " : " << std::fixed << std::setprecision(2) << voltage << std::scientific << std::endl;
    std::cout << "Temperature on channel " << channel << " : " << std::fixed << std::setprecision(2) << temperature << std::scientific << std::endl;
    std::cout << "Resistance on channel " << channel << " : " << std::fixed << std::setprecision(2) << resistance << std::scientific << std::endl;
    std::cout << std::endl;

    tla->disconnect();
}

int main() {
    do {
        test(TLA2024::MUX_1_GND);

        sleep(1);
    } while(true);
}
