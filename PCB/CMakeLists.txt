cmake_minimum_required(VERSION 3.12)
project(PCB)

set(CMAKE_CXX_STANDARD 14)

add_executable(PCB src/main.cpp src/heap.cpp include/heap.h src/process.cpp include/process.h)