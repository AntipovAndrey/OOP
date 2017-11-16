cmake_minimum_required(VERSION 3.8)
project(oop5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wl,--no-as-needed")

set(SOURCE_FILES main.cpp Cpp/IntegerFactorization.cpp Headers/IntegerFactorization.h Headers/IntegerFactorizationExceptions.h Cpp/FileStreamFactorization.cpp Headers/FileStreamFactorization.h Headers/Interfaces/Observer.h Headers/ConcurrentTask.h Headers/Interfaces/Iterable.h Headers/Interfaces/Stoppable.h Headers/Interfaces/Storable.h Headers/Interfaces/Readable.h Headers/Interfaces/Writable.h)
add_executable(oop4 ${SOURCE_FILES})
