cmake_minimum_required(VERSION 3.29)

project(
    TLA2024 
    LANGUAGES CXX)

add_library(lib TLA2024.cpp TLA2024.h)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE lib)