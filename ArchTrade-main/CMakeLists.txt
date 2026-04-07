cmake_minimum_required(VERSION 3.10)
project(ArchTrade LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(ArchTrade
  src/main.cpp
  src/Workload.cpp
)

target_include_directories(ArchTrade PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
