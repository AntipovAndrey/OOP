cmake_minimum_required(VERSION 3.8)
project(oop5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wl,--no-as-needed")

set(SOURCE_FILES main.cpp IntegerFactorization.cpp IntegerFactorization.h IntegerFactorizationExceptions.h FileStreamFactorization.cpp FileStreamFactorization.h ITaskObserver.h ConcurentTask.h)
add_executable(oop4 ${SOURCE_FILES})
