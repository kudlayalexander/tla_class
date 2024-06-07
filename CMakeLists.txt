cmake_minimum_required(VERSION 3.24)
project(battery_charger)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 11)

add_compile_options(
        -Wvla
        -Wall
        -Wextra
        -Weffc++
        -Wshadow
        -Wpedantic
        -Wcast-qual
        -Wcast-align
        -Wuninitialized
        -Wdouble-promotion
        -Wmissing-format-attribute

        -Wno-nested-anon-types
        -Wno-gnu-anonymous-struct
        -Wno-unused-local-typedefs
        -Wno-gnu-statement-expression
        -Wno-gnu-zero-variadic-macro-arguments

        -fno-rtti
        -ffast-math
        -fstack-protector-all

        -march=armv7l
        -mcpu=cortex-a9
        -mfpu=neon-fp16
        -mfloat-abi=hard
)

add_link_options(
        -pipe

        -fpic

        -Wl,-z,notext
        -Wl,-z,relro,-z,now
)

message(STATUS "Selected build type: ${CMAKE_BUILD_TYPE}")
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_compile_options(-O0 -g3 -ggdb)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    add_compile_options(-O3 -flto)
    add_link_options(-s -pie -fpie)
    add_compile_definitions(
            NDEBUG
            _FORTIFY_SOURCE=2
    )
else ()
    message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
endif ()

file(GLOB includes CONFIGURE_DEPENDS
        "*.h"
        "*/*.h")

file(GLOB sources CONFIGURE_DEPENDS
        "*.cpp"
        "*/*.cpp")

add_executable(${PROJECT_NAME} ${includes} ${sources})

find_package(TedixLibrary)
FindTedixLibrary(law testing 1.3.1-3-ga6630c7)
FindTedixLibrary(confparse stable 0.1.0-9-gaff32b4)