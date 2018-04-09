# cmake_minimum_required(VERSION <specify CMake version here>)
project(c__)

set(CMAKE_CXX_STANDARD 11)

add_executable(c__ main.cpp src/base/string_line.cc src/base/string_line.h src/base/table.cc src/base/table.h src/base/token.cc src/base/token.h src/base/error_info_exception.cc src/base/error_info_exception.h src/base/string_util.cc src/base/string_util.h src/lexical.cc src/lexical.h)