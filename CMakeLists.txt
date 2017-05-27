cmake_minimum_required(VERSION 2.8)
project(msgpackpp) # .sln
add_executable(tests tests/tests.cpp) # .vcxproj
include_directories(
    include
    tests
    )

