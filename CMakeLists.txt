cmake_minimum_required(VERSION 3.3)
project(Interpreter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

set(SOURCE_FILES interpreter_app.cpp Lex.cpp Lex.h parser.cpp parser.h scanner.cpp scanner.h lex_t.h ID.cpp ID.h)

add_executable(Interpreter ${SOURCE_FILES})