cmake_minimum_required(VERSION 3.10)

project (DonnerRT VERSION 0)

set (CMAKE_CXX_STANDARD 17)

set (CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(DonnerRT 
                Src/main.cpp
                Src/donMath.h
                Src/camera.h)

target_include_directories(DonnerRT PUBLIC
                        "${PROJECT_SOURCE_DIR}/Src")