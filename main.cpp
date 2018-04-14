#include "src/plugins/rang.hpp"
#include<iostream>
#include <fstream>
#include <memory>
#include<vector>
#include "src/base/string_line.h"
#include "src/base/table.h"
#include "src/base/error_info_exception.h"
#include "src/lexical.h"
#include "src/base/string_util.h"

using namespace std;
using namespace rang;


int main(int argc, char **args) {
    string filename = argc < 2 ? "code.java" : args[1];
    /**
     * 去除注释
     */
    auto lines = StringLine::convert_string(sutil::readCode(filename).get());
    /**
     * 加载 关键字、运算符、分隔符表
     */
    Table table;
    table.load_all();
    /**
     * 调用词法分析器
     */
    Lexical lexical(table);
    lexical.analyse(lines.first);

}

/*
 * TODO:
 * 1. 需要修复cout输出不完整 √
 * 2. 重新定义一下各种表，只用一种int，太单调
 * 3. 使用状态机重写词法分析器 √
 * 4. 熟练掌握C++使用 √×
 * 5. 遇到异常不进行判断 √
 */