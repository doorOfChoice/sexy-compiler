#include<iostream>
#include <fstream>
#include <memory>
#include<vector>
#include "src/base/StringLine.h"
#include "src/base/Table.h"
#include "src/base/ErrorInfoException.h"
#include "src/Lexical.h"

using namespace std;

/**
 * 从文件中读取数据
 * @param fname 文件名
 * @return 可自动回收的字符串指针
 */
shared_ptr<string> readCode(const string &fname) {
    fstream f;
    f.open(fname, ios::in);
    shared_ptr<string> s(new string());
    char buf[4096];
    if (f.is_open()) {
        while (!f.eof()) {
            f.getline(buf, sizeof(buf) / sizeof(char));
            s->append(buf);
            s->append("\n");
        }
        f.close();
    } else {
        cout << "File " << fname << " is not exist" << endl;
    }
    return s;
}


int main() {
    /**
     * 去除注释
     */
    auto lines = StringLine::convertString(readCode("code.java").get());
    for (shared_ptr<StringLine> &v:lines.first) {
        cout << v->getLine() << ":" << v->getText() ;
    }
    /**
     * 加载 关键字、运算符、分隔符表
     */
    Table table;
    table.loadAll();
    /**
     * 调用词法分析器
     */
    Lexical lexical(table);
    lexical.analyseLines(lines.first);
    cout << "~~~~~~~~errors" << endl;
    for (const auto &v : lexical.getErrors()) {
        cout << v.what() << endl;
    }
    cout << "~~~~~~~~end" << endl;
    for (const auto &v : lexical.getTokens()) {
        cout << v.to_string() << endl;
    }
}

/*
 * TODO:
 * 1. 需要修复cout输出不完整
 * 2. 重新定义一下各种表，只用一种int，太单调
 * 3. 使用状态机重写词法分析器
 * 4. 熟练掌握C++使用
 * 5. 遇到异常不进行判断
 */