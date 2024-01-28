cmake_minimum_required(VERSION 3.10)

project(nx_automation_meta_firmware)
set(CMAKE_C_STANDARD 99)

include_directories(include include/nxamf/pokecon)
file(GLOB SOURCES "src/*.c")
add_library(nxamf STATIC ${SOURCES})

file(GLOB NXMC2_SOURCES "src/nxmc2/*.c")
add_library(nxamf_nxmc2 STATIC ${NXMC2_SOURCES})

file(GLOB ORCA_SOURCES "src/orca/*.c")
add_library(nxamf_orca STATIC ${ORCA_SOURCES})

file(GLOB POKECON_SOURCES "src/pokecon/*.c")
add_library(nxamf_pokecon STATIC ${POKECON_SOURCES})

# Create test executable
file(GLOB TEST_SOURCES "test/*.c")
add_executable(test ${TEST_SOURCES})
target_link_libraries(test nxamf nxamf_nxmc2 nxamf_orca nxamf_pokecon)