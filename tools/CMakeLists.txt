cmake_minimum_required(VERSION 3.78)

# How to use
# 1. generate make file
#  cd vgszero/tools
#  cmake -S . -B build
# 2. build
#  cmake --build build --config Release
# 3. copy Release\*.exe files to each tools directories.

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# warning C4819
# https://learn.microsoft.com/en-us/cpp/build/reference/utf-8-set-source-and-executable-character-sets-to-utf-8?view=msvc-170
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

#=====================================================================================
# bmp2chr
#=====================================================================================
set(PROJECT_NAME bmp2chr)
file(GLOB SOURCES "${PROJECT_NAME}/*.c" "${PROJECT_NAME}/*.cpp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_NAME}")
project(${PROJECT_NAME} C CXX)
add_executable(${PROJECT_NAME} ${SOURCES}) 
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#=====================================================================================
# chkdif
#=====================================================================================
set(PROJECT_NAME chkdif)
file(GLOB SOURCES "${PROJECT_NAME}/*.c" "${PROJECT_NAME}/*.cpp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_NAME}")
project(${PROJECT_NAME} C CXX)
add_executable(${PROJECT_NAME} ${SOURCES}) 
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#=====================================================================================
# csv2bin
#=====================================================================================
set(PROJECT_NAME csv2bin)
file(GLOB SOURCES "${PROJECT_NAME}/*.c" "${PROJECT_NAME}/*.cpp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_NAME}")
project(${PROJECT_NAME} C CXX)
add_executable(${PROJECT_NAME} ${SOURCES}) 
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#=====================================================================================
# joypad
#=====================================================================================
# @todo

#=====================================================================================
# makebgm
#=====================================================================================
set(PROJECT_NAME makebgm)
file(GLOB SOURCES "${PROJECT_NAME}/*.c" "${PROJECT_NAME}/*.cpp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_NAME}")
project(${PROJECT_NAME} C CXX)
add_executable(${PROJECT_NAME} ${SOURCES}) 
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#=====================================================================================
# makepkg
#=====================================================================================
set(PROJECT_NAME makepkg)
file(GLOB SOURCES "${PROJECT_NAME}/*.c" "${PROJECT_NAME}/*.cpp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_NAME}")
project(${PROJECT_NAME} C CXX)
add_executable(${PROJECT_NAME} ${SOURCES}) 
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#=====================================================================================
# makerom
#=====================================================================================
set(PROJECT_NAME makerom)
file(GLOB SOURCES "${PROJECT_NAME}/*.c" "${PROJECT_NAME}/*.cpp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_NAME}")
project(${PROJECT_NAME} C CXX)
add_executable(${PROJECT_NAME} ${SOURCES}) 
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#=====================================================================================
# makese
#=====================================================================================
set(PROJECT_NAME makese)
file(GLOB SOURCES "${PROJECT_NAME}/*.c" "${PROJECT_NAME}/*.cpp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_NAME}")
project(${PROJECT_NAME} C CXX)
add_executable(${PROJECT_NAME} ${SOURCES}) 
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#=====================================================================================
# vgsasm
#=====================================================================================
set(PROJECT_NAME vgsasm)
file(GLOB SOURCES "${PROJECT_NAME}/*.c" "${PROJECT_NAME}/*.cpp" "${PROJECT_NAME}/src/*.c" "${PROJECT_NAME}/src/*.cpp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_NAME}")
project(${PROJECT_NAME} C CXX)
add_executable(${PROJECT_NAME} ${SOURCES}) 
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#=====================================================================================
# vgsmml
#=====================================================================================
set(PROJECT_NAME vgsmml)
file(GLOB SOURCES "${PROJECT_NAME}/*.c" "${PROJECT_NAME}/*.cpp" "${PROJECT_NAME}/src/*.c" "${PROJECT_NAME}/src/*.cpp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_NAME}")
project(${PROJECT_NAME} C CXX)
add_executable(${PROJECT_NAME} ${SOURCES}) 
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#=====================================================================================
# vgsplay
#=====================================================================================
set(PROJECT_NAME vgsplay)
file(GLOB SOURCES "${PROJECT_NAME}/src/*.c"
    "${PROJECT_NAME}/src/BufferQueue.cpp"
    "${PROJECT_NAME}/src/sound-sdl2.cpp"
    "${PROJECT_NAME}/src/vgsplay.cpp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_NAME}")
project(${PROJECT_NAME} C CXX)
add_executable(${PROJECT_NAME} ${SOURCES}) 
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# SDL2
# Memo) Windows environment
#       set the path to the environment variable SDL2_ROOT.
find_package(SDL2 REQUIRED)
include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
