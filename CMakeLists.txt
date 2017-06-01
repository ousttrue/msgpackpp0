cmake_minimum_required(VERSION 2.8)
project(msgpackpp) # .sln
add_executable(tests 
    include/msgpackpp.h
    tests/tests.cpp
    ) # .vcxproj
include_directories(
    include
    tests
    )

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-unknown-pragmas -Wno-switch")

add_executable(sample 
    include/msgpackpp.h
    samples/sample.cpp
    ) # .vcxproj
include_directories(
    include
    )

