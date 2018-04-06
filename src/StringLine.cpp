//
// Created by dawndevil on 2018/4/6.
//
#include <iostream>
#include <vector>
#include <memory> #include <vector>
#include "StringLine.h"

std::vector<std::shared_ptr<StringLine>> StringLine::convertString(std::string *s) {
    std::vector<std::shared_ptr<StringLine>> sls;
    int lineNumber = 1;
    auto it = s->begin();
    //StringLine
    auto sl = std::make_shared<StringLine>(lineNumber, std::string());
    //判断是否已经设置行数
    bool countLine = false;
    while (it != s->end()) {
        /**
         * 去除//类型的注释
         */
        if (*it == '/' && *(it + 1) == '/') {
            while (*(it + 1) != '\n')++it;
        /**
         * 去除/*类型的注释
         */
        } else if (*it == '/' && *(it + 1) == '*') {
            while (*it != '*' || *(it + 1) != '/') {
                if (*it == '\n')++lineNumber;
                ++it;
            }
            it += 1;
        /**
         * 偶遇换行符考虑存储行号和该行的数据
         */
        } else if (*it == '\n') {
            if (!sl->getText().empty()) {
                sls.push_back(sl);
                sl = std::make_shared<StringLine>(lineNumber, std::string());
                countLine = false;
            }
            ++lineNumber;
        /**
         * 读取到普通字符
         */
        } else {
            if(!countLine) {
                sl->setLine(lineNumber);
                countLine = true;
            }
            sl->getText().push_back(*it);
        }
        ++it;
    }
    return sls;
}
