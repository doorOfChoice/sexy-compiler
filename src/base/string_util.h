/**
 * 字符串单元类，可以对特殊字符串或者字符进行识别
 * 包括判断是否是数字或者是否是标识符字符
 */

#ifndef C_STRINGUTIL_H
#define C_STRINGUTIL_H


#include <string>

namespace stil {
    /**
     * 去除两边的空白符号，包括\t和\n
     * @return
     */
    std::string trim(const std::string &);

    /**
     * 判断是否是关键字
     * @return
     */
    bool is_key(const char &, bool = false);

    /**
     * 判断是否是空白，包括\n
     * @return
     */
    bool is_blank(const char &);

    /**
     * 判断是否是八进制
     * @return
     */
    bool is_octal(const char &);
}


#endif //C_STRINGUTIL_H
