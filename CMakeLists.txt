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

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # using GCC
    set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-unknown-pragmas -Wno-switch")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # using Visual Studio C++
endif()

add_executable(sample 
    include/msgpackpp.h
    samples/sample.cpp
    ) # .vcxproj
include_directories(
    include
    )

