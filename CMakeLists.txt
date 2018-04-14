 cmake_minimum_required(VERSION 3.10)
project(run)

set(BOOST_ROOT "/usr/local/include/boost")

#添加头文件搜索路径
include_directories(/usr/local/include)

#添加库文件搜索路径
link_directories(/usr/local/lib)

set(CMAKE_CXX_STANDARD 17)


add_executable(run src/main.cpp src/plugins/json.hpp src/plugins/rang.hpp src/base/string_line.cc src/base/string_line.h src/base/table.cc src/base/table.h src/base/token.cc src/base/token.h src/base/error_info_exception.cc src/base/error_info_exception.h src/base/string_util.cc src/base/string_util.h src/lexical.cc src/lexical.h src/base/symbol.h)

target_link_libraries(run boost_thread boost_system pthread)
