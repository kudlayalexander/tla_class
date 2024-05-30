cmake_minimum_required(VERSION 3.16)

project(
    TLA2024 
    LANGUAGES CXX)

add_library(tla2024 TLA2024.cpp TLA2024.h)

add_executable(test test_TLA2024.cpp)

target_link_libraries(test PRIVATE tla2024)  

