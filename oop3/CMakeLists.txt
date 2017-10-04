cmake_minimum_required(VERSION 3.8)
project(oop3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Wav.cpp Wav.h WavException.h)
add_executable(oop3 ${SOURCE_FILES})