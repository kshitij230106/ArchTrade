cmake_minimum_required(VERSION 3.10)

project(ArchTrade)

set(CMAKE_CXX_STANDARD 17)

add_executable(ArchTrade_web

    src/web_simulator.cpp
    src/Workload.cpp
)