cmake_minimum_required(VERSION 3.22)
project(User_Level_Thread_Management_Library)

set(CMAKE_CXX_STANDARD 14)

# Include the main source directory
include_directories(src)

# Create the main executable from the source files
add_executable(User_Level_Thread_Management_Library
        src/Thread.cpp
        src/Thread.h
        src/uthreads.cpp
        src/uthreads.h

        # Uncomment the relevant lines to compile and run specific tests:

        # tests/ex2_test.cpp
        # tests/test0_sanity.cpp
        # tests/test2_two_thread.cpp
        # tests/jona1.cpp
        # tests/jona2.cpp
        # tests/jona3.cpp
        # tests/jona4.cpp
        # tests/jona5.cpp
)


