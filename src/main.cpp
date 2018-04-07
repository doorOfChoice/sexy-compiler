#include<iostream>
#include <fstream>
#include <memory>
#include<vector>
#include "StringLine.h"
#include "Table.h"
#include "ErrorInfoException.h"

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

Table table;
vector<ErrorInfoException> errorInfos;

int main() {
    try {
        auto lines = StringLine::convertString(readCode("code.java").get());
        for (shared_ptr<StringLine> &v:lines) {
            cout << v->getLine() << ":" << v->getText() << endl;
        }
        table.loadAll();
        table.analyseLines(lines);
        for (const auto &v : table.getTokens()) {
            cout << v.to_string() << endl;
        }
    } catch (ErrorInfoException &e) {
        errorInfos.push_back(e);
    }
}

/*
 * TODO:
 * 1. 需要修复cout输出不完整
 * 2. 重新定义一下各种表，只用一种int，太单调
 */