cmake_minimum_required(VERSION 3.2)
project(Runner)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static-libgcc -static-libstdc++")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "C:\\ClionProjects\\Binaries")

set(SOURCE_FILES main.cpp DataParser.cpp DataParser.h)
add_executable(Runner ${SOURCE_FILES})
file(COPY doeTable.dat DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY Driver DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY LabView DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})


set(CMAKE_RUNTIME_TEST_OUTPUT_DIRECTORY "C:\\ClionProjects\\Test")
set(SOURCE_TEST_FILES Test/test.cpp DataParser.cpp DataParser.h)
add_executable(TestParser ${SOURCE_TEST_FILES})
file(COPY doeTable.dat DESTINATION ${CMAKE_RUNTIME_TEST_OUTPUT_DIRECTORY})