cmake_minimum_required(VERSION 3.8)
project(oop4)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp IntegerFactorization.cpp IntegerFactorization.h IntegerFactorizationExceptions.h FileStreamFactorization.cpp FileStreamFactorization.h)
add_executable(oop4 ${SOURCE_FILES})