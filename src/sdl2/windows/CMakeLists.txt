cmake_minimum_required(VERSION 3.78)

# How to use
# 1. generate make file
#  cd vgszero/src/sdl2/windows
#  cmake -S . -B build
# 2. build
#  cmake --build build

set(PROJECT_NAME vgs0)
project(${PROJECT_NAME} C CXX)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

#=====================================================================================
# Source files
#=====================================================================================

set(BASE_DIR "../..")

set(PRIVATE_SOURCE_FILES
    ${BASE_DIR}/sdl2/vgs0sdl2.cpp
    ${BASE_DIR}/core/vgstone.c
    ${BASE_DIR}/core/vgs0math.c
    ${BASE_DIR}/core/nsf/xgm/player/nsf/nsf.cpp
    ${BASE_DIR}/core/nsf/xgm/devices/Audio/rconv.cpp
    ${BASE_DIR}/core/nsf/xgm/devices/CPU/nes_cpu.cpp
    ${BASE_DIR}/core/nsf/xgm/devices/Memory/nes_bank.cpp
    ${BASE_DIR}/core/nsf/xgm/devices/Memory/nes_mem.cpp
    ${BASE_DIR}/core/nsf/xgm/devices/Memory/nsf2_vectors.cpp
    ${BASE_DIR}/core/nsf/xgm/devices/Misc/nsf2_irq.cpp
    ${BASE_DIR}/core/nsf/xgm/devices/Sound/nes_apu.cpp
    ${BASE_DIR}/core/nsf/xgm/devices/Sound/nes_dmc.cpp
    ${BASE_DIR}/core/nsf/xgm/devices/Sound/nes_vrc6.cpp
    ${BASE_DIR}/core/nsf/xgm/devices/Sound/rom_tndtable.c
)
set(PRIVATE_HEADER_FILES
    ${BASE_DIR}/core/vgs0def.h
    ${BASE_DIR}/core/perlinnoise.hpp
    ${BASE_DIR}/core/vdp.hpp
    ${BASE_DIR}/core/vgs0.hpp
    ${BASE_DIR}/core/vgsdecv.hpp
    ${BASE_DIR}/core/z80.hpp
)
# @todo
#file(GLOB_RECURSE PRIVATE_HEADER_FILES RELATIVE "${CMAKE_SOURCE_DIR}/../.." "core/*.hpp")

include_directories(${PROJECT_NAME} ${BASE_DIR}/core)
add_executable(${PROJECT_NAME} ${PRIVATE_SOURCE_FILES} ${PRIVATE_HEADER_FILES})
if(WIN32)
    # Set up a startup project (for VC++)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif(WIN32)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#=====================================================================================
# Filter for VC++
#=====================================================================================
if(WIN32)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    source_group(TREE ${BASE_DIR}  FILES ${PRIVATE_SOURCE_FILES})
    source_group(TREE ${BASE_DIR}  FILES ${PRIVATE_HEADER_FILES})
endif(WIN32)

#=====================================================================================
# SDL2
#-------------------------------------------------------------------------------------
# Memo) Windows environment
#       set the path to the environment variable SDL2_ROOT.
find_package(SDL2 REQUIRED)
include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
#=====================================================================================
