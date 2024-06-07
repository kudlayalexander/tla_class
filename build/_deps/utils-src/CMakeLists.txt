cmake_minimum_required(VERSION 3.24)
project(utils)

set(CMAKE_CXX_STANDARD 20)

set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)

set(WARNING_FLAGS "-Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wvla -Wshadow -Wno-unused-local-typedefs")
set(WARNING_FLAGS "${WARNING_FLAGS} -Wmissing-format-attribute -Wuninitialized -Wdouble-promotion -Wno-gnu-statement-expression")
set(WARNING_FLAGS "${WARNING_FLAGS} -Wno-gnu-string-literal-operator-template -Wno-gnu-anonymous-struct -Wno-nested-anon-types")

set(COMPILER_FEATURES "-fstack-protector-all")

set(COMMON_FLAGS "${COMPILER_FEATURES} -march=armv7l -O3 -flto -DNDEBUG -D_FORTIFY_SOURCE=2 ${WARNING_FLAGS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Weffc++ ${COMMON_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 ${COMMON_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fpie -s -pipe -fpic -Wl,-z,relro,-z,now -Wl,-z,notext -Wl,-no-undefined")

include_directories(.)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

file(GLOB includes CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*/*/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*/*/*/*.h")

file(GLOB sources CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

#add_executable(${PROJECT_NAME} ${includes} ${sources} main.cpp)
add_library(${PROJECT_NAME} STATIC ${includes} ${sources})

target_link_libraries(${PROJECT_NAME} rt)
