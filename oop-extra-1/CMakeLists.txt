project(oop_extra_1)

set(CMAKE_CXX_STANDARD 11)

if(MSVC)
    set(MAIN_CPP_FNAME "main_windows.cpp" )
else()
    set(MAIN_CPP_FNAME "main_linux.cpp" )
endif()

set(SOURCE_FILES ${MAIN_CPP_FNAME} Source/RegistersConverter.cpp Headers/RegistersConverter.h Source/RegistersParser.cpp Headers/RegistersParser.h)
add_executable(oop_extra_1 ${SOURCE_FILES})