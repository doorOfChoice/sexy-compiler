#include "src/plugins/rang.hpp"
#include<iostream>
#include <fstream>
#include <memory>
#include<vector>
#include "src/base/string_line.h"
#include "src/base/table.h"
#include "src/base/error_info_exception.h"
#include "src/lexical.h"

using namespace std;
using namespace rang;

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

void print_code(const vector<shared_ptr<StringLine>> &v, const vector<ErrorInfoException> &errors) {
    system("clear");
    for (const auto &it : v) {
        cout << fg::yellow << it->get_line() << fg::reset << ": ";
        for (int i = 0; i < it->get_text().size(); ++i) {
            int column = i + 1;
            if (!errors.empty()) {
                if (errors[0].get_row() == it->get_line()) {
                    cout << bg::blue;
                    if (column >= errors[0].get_column_begin() && column <= errors[0].get_column())
                        cout << bg::red;
                }
            }
            cout << it->get_text()[i];
            if (!errors.empty()) cout << bg::reset;
        }
    }
}

/**
 * 打印token表到token.txt文件下
 * @param v
 */
void print_token(const vector<Token> &v) {
    fstream f;
    f.open("token.txt", ios::out);
    for (const auto &it : v) {
        f << it.to_string() << endl;
    }
    f.close();
}

void print_errors(const vector<ErrorInfoException> &errors) {
    cout << style::bold << bg::red << "\t\t\t错误表" << style::reset << bg::reset << endl;
    cout << fg::red;
    for (const auto &it : errors) {
        cout << it.what() << endl;
    }
    cout << fg::reset;
}

int main(int argc, char **args) {
    string filename = argc < 2 ? "code.java" : args[1];

    /**
     * 去除注释
     */
    auto lines = StringLine::convert_string(readCode(filename).get());
    /**
     * 加载 关键字、运算符、分隔符表
     */
    Table table;
    table.load_all();
    /**
     * 调用词法分析器
     */
    lexical lexical(table);
    lexical.analyse(lines.first);


    print_code(lines.first, lexical.get_errors());
    print_errors(lexical.get_errors());
    print_token(lexical.get_tokens());
}

/*
 * TODO:
 * 1. 需要修复cout输出不完整
 * 2. 重新定义一下各种表，只用一种int，太单调
 * 3. 使用状态机重写词法分析器
 * 4. 熟练掌握C++使用
 * 5. 遇到异常不进行判断
 */