# cmake_minimum_required(VERSION <specify CMake version here>)
project(c__)

set(CMAKE_CXX_STANDARD 11)

add_executable(c__ main.cpp src/base/StringLine.cpp src/base/StringLine.h src/base/Table.cpp src/base/Table.h src/base/Token.cpp src/base/Token.h src/base/ErrorInfoException.cpp src/base/ErrorInfoException.h src/base/StringUtil.cpp src/base/StringUtil.h src/Lexical.cpp src/Lexical.h)