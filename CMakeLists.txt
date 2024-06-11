cmake_minimum_required(VERSION 3.24)
project(bch)

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

add_compile_definitions(FMT_HEADER_ONLY)

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

include_directories(${CMAKE_SOURCE_DIR}/app)

file(GLOB includes CONFIGURE_DEPENDS
"app/*.h"
"app/*/*.h"
"app/*/*/*.h")

file(GLOB sources CONFIGURE_DEPENDS
"app/*.cpp"
"app/*/*.cpp"
"app/*/*/*.cpp")

add_executable(${PROJECT_NAME} ${includes} ${sources})

find_package(TedixLibrary)

FindTedixLibrary(elaw testing 1.1.1-6-g0d99ca5)
FindTedixLibrary(tpapi testing 1.2.14-9-g469ba0c)
FindTedixLibrary(magic_enum stable v0.9.5-1-g7f3b4b0)
FindTedixLibrary(rapidjson stable 1.1.0)

execute_process(COMMAND bash -c "cat ${CMAKE_SOURCE_DIR}/debian/bch/etc/bch/config.json | tr -d '\n' | tr -d ' '" OUTPUT_VARIABLE DEFAULT_CONFIG)
configure_file(${CMAKE_SOURCE_DIR}/app/config/JsonConfigManager.h.in ${CMAKE_SOURCE_DIR}/app/config/JsonConfigManager.h)