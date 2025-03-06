cmake_minimum_required(VERSION 3.22)
project(Projec_4)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Projec_4
        MemoryConstants.h
        PhysicalMemory.cpp
        PhysicalMemory.h
        VirtualMemory.cpp
        VirtualMemory.h
        SimpleTest.cpp
)
