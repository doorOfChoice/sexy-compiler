#include "plugins/rang.hpp"
#include<iostream>
#include <fstream>
#include <memory>
#include<vector>
#include "base/string_line.h"
#include "base/table.h"
#include "base/error_info_exception.h"
#include "lexical.h"
#include "base/string_util.h"
#include "plugins/crow_all.h"

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
    table.load_all("data.json");
    /**
     * 调用词法分析器
     */
    Lexical lexical(table);
    lexical.analyse(lines.first);
//    crow::SimpleApp app;
//
//    CROW_ROUTE(app, "/")([](){
//        return "Hello world";
//    });
//
//    app.port(18080).multithreaded().run();
    return 0;
}

/*
 * TODO:
 * 1. 需要修复cout输出不完整 √
 * 2. 重新定义一下各种表，只用一种int，太单调
 * 3. 使用状态机重写词法分析器 √
 * 4. 熟练掌握C++使用 √×
 * 5. 遇到异常不进行判断 √
 */