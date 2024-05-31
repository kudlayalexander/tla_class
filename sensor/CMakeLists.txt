cmake_minimum_required(VERSION 3.16)

project(
    TLA2024 
    LANGUAGES CXX)

add_library(sensor sensor.cpp sensor.cpp)

add_executable(test test.cpp)

target_link_libraries(test PRIVATE sensor)  