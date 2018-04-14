//
// Created by dawndevil on 2018/4/7.
//

#include <fstream>
#include <iostream>
#include "string_util.h"


/**
 * 从文件中读取数据
 * @param fname 文件名
 * @return 可自动回收的字符串指针
 */
std::shared_ptr<std::string> sutil::readCode(const std::string &fname) {
    std::fstream f;
    f.open(fname, std::ios::in);
    std::shared_ptr<std::string> s(new std::string());
    char buf[4096];
    if (f.is_open()) {
        while (!f.eof()) {
            f.getline(buf, sizeof(buf) / sizeof(char));
            s->append(buf);
            s->append("\n");
        }
        f.close();
    } else {
        std::cout << "File " << fname << " is not exist" << std::endl;
    }
    return s;
}

/**
 * 去除包含\n在内的Left AND Right 空白字符
 * @param s
 * @return
 */
std::string sutil::trim(const std::string &s) {
    std::string str = s;
    str.replace(0, str.size() - 1, '\n', ' ');
    str.erase(0, str.find_first_not_of(' '));
    str.erase(str.find_last_not_of(' ') + 1, str.size());
    str.erase(0, str.find_first_not_of('\t'));
    str.erase(str.find_last_not_of('\t') + 1, str.size());
    return str;
}


std::string sutil::trim_quotes(const std::string &s){
    std::string str = s;
    str.erase(0, str.find_first_not_of('\"'));
    str.erase(str.find_last_not_of('\"') + 1, str.size());
    return str;
}

/**
 * 标识符识别
 * @param ch
 * @param isHead
 * @return
 */
bool sutil::is_key(const char &ch, bool isHead) {
    bool head = isalpha(ch) || ch == '_' || ch == '$';
    return isHead ? head : head || isdigit(ch);
}

bool sutil::is_blank(const char &ch) {
    return isblank(ch) || ch == '\n';
}

bool sutil::is_octal(const char &ch) {
    return ch >= '0' && ch < '8';
}