cmake_minimum_required(VERSION 3.27)
project(AGH___ZPO___NetSim)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

set(SOURCE_FILES
        src/package.cpp
)
add_executable(AGH___ZPO___NetSim main.cpp
        include/package.hpp
        src/package.cpp)
