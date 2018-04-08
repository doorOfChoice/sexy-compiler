# cmake_minimum_required(VERSION <specify CMake version here>)
project(c__)

set(CMAKE_CXX_STANDARD 11)

add_executable(c__ main.cpp src/StringLine.cpp src/StringLine.h src/Table.cpp src/Table.h src/Token.cpp src/Token.h src/ErrorInfoException.cpp src/ErrorInfoException.h src/StringUtil.cpp src/StringUtil.h)