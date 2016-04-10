cmake_minimum_required(VERSION 3.3)
project(Interpreter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES scanner.cpp scanner.h parser.cpp parser.h)
add_executable(Interpreter ${SOURCE_FILES})