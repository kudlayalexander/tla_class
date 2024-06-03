cmake_minimum_required(VERSION 3.16)

project(
    TLA2024 
    LANGUAGES CXX)

add_library(battery battery.cpp battery.h)
add_library(tla2024 ../tla2024_adc1015/TLA2024.h ../tla2024_adc1015/TLA2024.cpp)
add_executable(test test_battery.cpp)

target_link_libraries(test PRIVATE battery tla2024)  