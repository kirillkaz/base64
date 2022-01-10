cmake_minimum_required(VERSION 3.20)
project(base64)

set(CMAKE_CXX_STANDARD 23)

add_library(BASE64LIB STATIC base64lib.cpp base64lib.h)

add_executable(base64 main.cpp base64lib.cpp base64lib.h)

target_link_libraries(base64 BASE64LIB)

add_subdirectory(tests/googletest)
enable_testing()
#include_directories(${STACK_INCLUDE_DIR})
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
target_link_libraries(base64 gtest gtest_main)