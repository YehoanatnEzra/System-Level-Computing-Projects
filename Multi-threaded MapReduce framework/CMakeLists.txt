cmake_minimum_required(VERSION 3.22)
project(Ex3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -g")

# Include the current directory to find all headers (including MapReduceClient.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Define source files
set(SOURCES
        Barrier.cpp
        MapReduceFramework.cpp
)

# Create a static library for the framework
add_library(MapReduceFramework STATIC ${SOURCES})

# Define test executable
add_executable(
        test_mapreduce
        # test4-1_thread_4_process.cpp
        # test1-1_thread_1_process.cpp
)

target_link_libraries(test_mapreduce PRIVATE MapReduceFramework)
