cmake_minimum_required(VERSION 3.24)
project(confparse)

set(CMAKE_CXX_STANDARD 20)

set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)

set(CMAKE_CXX_STANDARD 20)

set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)

set(WARNING_FLAGS "-Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wvla -Wshadow -Wno-unused-local-typedefs -Wno-gnu-zero-variadic-macro-arguments")
set(WARNING_FLAGS "${WARNING_FLAGS} -Wmissing-format-attribute -Wuninitialized -Wdouble-promotion -Wno-gnu-statement-expression")
set(WARNING_FLAGS "${WARNING_FLAGS} -Wno-gnu-string-literal-operator-template -Wno-gnu-anonymous-struct -Wno-nested-anon-types")

set(COMPILER_FEATURES "-fstack-protector-all -fno-rtti -ffast-math -march=armv7l -ffast-math -mcpu=cortex-a9 -mfpu=neon-vfpv4 -mfloat-abi=hard")

add_compile_definitions(FMT_HEADER_ONLY)

message(STATUS "Selected build type: ${CMAKE_BUILD_TYPE}")
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(COMMON_FLAGS "${COMPILER_FEATURES} -O0 -g3 -ggdb ${WARNING_FLAGS}")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    add_compile_definitions(
            NDEBUG
            _FORTIFY_SOURCE=2)
    set(COMMON_FLAGS "${COMPILER_FEATURES} -O3 -flto ${WARNING_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pie -fpie -s -Wl,-no-undefined")
else()
    message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Weffc++ ${COMMON_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 ${COMMON_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pipe -fpic -Wl,-z,relro,-z,now -Wl,-z,notext")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIC")

include_directories(.)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

file(GLOB includes CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/inc/*/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/inc/*/*/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/inc/*/*/*/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/inc/*/*/*/*/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/inc/*/*/*/*/*/*.h")

file(GLOB sources CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} STATIC ${includes} ${sources})
#add_executable(${PROJECT_NAME} ${includes} ${sources} main.cpp)

find_package(TedixLibrary)
FindTedixLibrary(etl stable 20.35.14)
FindTedixLibrary(rapidjson stable 1.1.0)
FindTedixLibrary(logger stable 1.0.0-19-gab81799)
