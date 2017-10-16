cmake_minimum_required(VERSION 3.8)
project(oop_extra_1)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp RegistersConverter.cpp RegistersConverter.h RegistersParser.cpp RegistersParser.h)
add_executable(oop_extra_1 ${SOURCE_FILES})