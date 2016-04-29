cmake_minimum_required(VERSION 3.3)
project(Interpreter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

include_directories(sub_classes)
set(CLASSES sub_classes/Lex.cpp sub_classes/Lex.h sub_classes/lex_t.h sub_classes/ID.cpp sub_classes/ID.h sub_classes/Exception.cpp sub_classes/Exception.h sub_classes/RPN.cpp sub_classes/RPN.h)
add_library(MY_LIB STATIC ${CLASSES})

set(SOURCE_FILES interpreter_app.cpp parser.cpp parser.h scanner.cpp scanner.h)
set(TEST_FILES prog test_macros)

add_executable(Interpreter ${SOURCE_FILES} ${CLASSES})