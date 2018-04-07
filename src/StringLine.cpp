//
// Created by dawndevil on 2018/4/6.
//
#include <iostream>
#include <vector>
#include <memory>
#include <vector>
#include "StringLine.h"
#include "ErrorInfoException.h"
/**
 * 去除包含\n在内的Left AND Right 空白字符
 * @param s
 * @return
 */
std::string trim(const std::string &s) {
    std::string str = s;
    str.replace(0, str.size() - 1, '\n', ' ');
    str.erase(0, str.find_first_not_of(' '));
    str.erase(str.find_last_not_of(' ') + 1, str.size());
    return str;
}

std::vector<std::shared_ptr<StringLine>> StringLine::convertString(std::string *s) {
    std::vector<std::shared_ptr<StringLine>> sls;
    int lineNumber = 1;
    auto it = s->begin();
    //StringLine
    auto sl = std::make_shared<StringLine>(lineNumber, std::string());
    int state = 0;
    bool isAnnotation = false;
    while (it != s->end()) {
        switch (state) {
            //初始读取状态，判断是注释还是普通字符
            case 0: {
                if (*it == '/')state = 1;
                else {
                    state = 7;
                    --it;
                }
                break;
            }
             //判断第二个/，或者第一个*
            case 1: {
                if (*it == '/')state = 2;
                else if (*it == '*')state = 4;
                else {
                    state = 0;
                    --it;
                }
                break;
            }
             // 判断是//注释，还是换行停止
            case 2: {
                if (*it == '\n') {
                    state = 3;
                    --it;
                } else {
                    sl->getText().push_back(' ');
                    state = 2;
                }
                break;
            }
             //处理已经读取了一行的情况
            case 3: {
                if (!trim(sl->getText()).empty()) {
                    sls.push_back(sl);
                }
                sl = std::make_shared<StringLine>(lineNumber, std::string());
                ++lineNumber;
                state = isAnnotation? 4 : 0;
                break;
            }
             // 处理/*注释
            case 4: {
                if (*it == '*')state = 5;
                else {
                    isAnnotation = true;
                    if (*it == '\n'){
                        state = 3;
                        --it;
                    }else {
                        sl->getText().push_back(' ');
                        state = 4;
                    }
                }
                break;
            }
             // 判断*后面是否是/
            case 5: {
                if (*it == '/') {
                    sl->getText().append("    ");
                    isAnnotation = false;
                    state = 0;
                } else {
                    state = 4;
                    --it;
                }
                break;
            }
             //读取普通字符
            case 7: {
                if (*it == '/') {
                    state = 0;
                    --it;
                } else if (*it != '\n') {
                        sl->setLine(lineNumber);
                    sl->getText().push_back(*it);
                    state = 7;
                } else {
                    state = 3;
                    --it;
                }
            }
            default:;
        }
        ++it;
    }
    return sls;
}