/**
 * 字符串单元类，可以对特殊字符串或者字符进行识别
 * 包括判断是否是数字或者是否是标识符字符
 */

#ifndef C_STRINGUTIL_H
#define C_STRINGUTIL_H


#include <string>

class StringUtil {
public:
    static std::string trim(const std::string &);
    static bool is_key(char , bool = false);
    static bool is_blank(char);
};


#endif //C_STRINGUTIL_H
