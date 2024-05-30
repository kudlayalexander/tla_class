cmake_minimum_required(VERSION 3.16)

project(
    TLA2024 
    LANGUAGES CXX)

add_library(temperature_sensor temperature_sensor.cpp temperature_sensor.cpp)

add_executable(test test_sensor.cpp)

target_link_libraries(test PRIVATE temperature_sensor)  