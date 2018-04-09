//
// Created by dawndevil on 2018/4/7.
//

#include "string_util.h"

/**
 * 去除包含\n在内的Left AND Right 空白字符
 * @param s
 * @return
 */
std::string StringUtil::trim(const std::string &s) {
    std::string str = s;
    str.replace(0, str.size() - 1, '\n', ' ');
    str.erase(0, str.find_first_not_of(' '));
    str.erase(str.find_last_not_of(' ') + 1, str.size());
    str.erase(0, str.find_first_not_of('\t'));
    str.erase(str.find_last_not_of('\t') + 1, str.size());
    return str;
}

/**
 * 标识符识别
 * @param ch
 * @param isHead
 * @return
 */
bool StringUtil::is_key(char ch, bool isHead) {
    bool head = isalpha(ch) || ch == '_' || ch == '$';
    return isHead ? head : head || isdigit(ch);
}

bool StringUtil::is_blank(char ch) {
    return isblank(ch) || ch == '\n';
}
