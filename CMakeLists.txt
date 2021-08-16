cmake_minimum_required(VERSION 3.14)
project(msgpackpp)

set(CMAKE_CXX_STANDARD 17)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  add_compile_options(-Wall -Wno-unknown-pragmas -Wno-switch)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
  add_compile_options(/source-charset:utf-8)
endif()

add_library(msgpackpp INTERFACE)
target_include_directories(msgpackpp
                           INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_compile_options(msgpackpp INTERFACE /wd4267)

subdirs(samples catch tests)
