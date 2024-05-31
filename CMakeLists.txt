cmake_minimum_required(VERSION 3.18)

project(
    TLA2024 
    LANGUAGES CXX)

add_library(tla2024 tla2024_adc1015/TLA2024.cpp tla2024_adc1015/TLA2024.h)
add_library(temperature_sensor temperature_sensor/temperature_sensor.cpp temperature_sensor/temperature_sensor.h)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE tla2024 temperature_sensor)
