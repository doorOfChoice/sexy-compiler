#include<iostream>
#include <fstream>
#include <memory>
#include<vector>
#include "StringLine.h"

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
    char buf[100];
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
    auto v = StringLine::convertString(readCode("code.java").get());
    for (const auto &it : v) {
        cout << it->getLine() << " " << *it->getText() << endl;
    }
}
