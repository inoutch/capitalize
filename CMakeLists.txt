cmake_minimum_required(VERSION 3.7)
project(capitalize)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp capthread.cpp capthread.h capfork.cpp capfork.h capitalize.h)
add_executable(capitalize ${SOURCE_FILES})