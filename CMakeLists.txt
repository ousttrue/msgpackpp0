cmake_minimum_required(VERSION 2.8)
project(msgpackpp) # .sln

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # using GCC
    set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wno-unknown-pragmas -Wno-switch")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # using Visual Studio C++
	#set(CMAKE_CXX_FLAGS "/std:c++latest /EHsc")
endif()

add_executable(tests 
    include/msgpackpp.h
    tests/msgpack.cpp
    tests/serialize.cpp
    tests/rpc.cpp
    ) # .vcxproj
include_directories(
    include
    tests
    )

add_executable(sample 
    include/msgpackpp.h
    samples/sample.cpp
    ) # .vcxproj
include_directories(
    include
    )
