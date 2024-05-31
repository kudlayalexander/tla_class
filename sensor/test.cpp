#include <chrono>
#include "sensor.h"
#include <iostream>
#include <iomanip>

void test(float voltage) {
    std::unique_ptr<Sensor> sensor {std::make_unique<Sensor>()};
    float resistance, temperature;
    sensor->setVoltage(voltage);

    resistance = sensor->getResistance();
    temperature = sensor->getTemperature();

    std::cout << "Resistance for voltage " << voltage << " : " << std::fixed  << resistance << std::endl;
    std::cout << "Temperature for voltage " << voltage << " : " << std::fixed  << temperature << std::endl;
    std::cout << "--------------" << std::endl;
}

int main() {
    std::unique_ptr<Sensor> sensor {std::make_unique<Sensor>()};
    auto start = std::chrono::high_resolution_clock::now();

    test(0.9f);
    test(0.984782f);
    test(1.2f);
    test(1.5f);
    test(1.8);
    test(2.1f);
    test(2.4f);
    test(2.6f);
    test(2.810699f);
    test(2.9f);
    test(3.0f);

    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(finish-start).count() << "us\n";
}