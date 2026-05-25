cmake_minimum_required(VERSION 3.10)
<<<<<<< HEAD

project(ArchTrade)

set(CMAKE_CXX_STANDARD 17)

add_executable(ArchTrade_web

    src/web_simulator.cpp
    src/Workload.cpp
)
=======
project(ArchTrade LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(ArchTrade
  src/main.cpp
  src/Workload.cpp
)

target_include_directories(ArchTrade PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
>>>>>>> fe6656cc5573ce4a019edd25d5b2d55b50053d6c
