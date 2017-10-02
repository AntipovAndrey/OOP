cmake_minimum_required(VERSION 3.8)
project(oop1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp IniParser.cpp IniParser.h IniException.h)
add_executable(oop1 ${SOURCE_FILES})