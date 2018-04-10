#include "token.h"
#include <ostream>


Token::Token(int row, int column, int type, const std::string &name) : row(row), column(column), type(type),
                                                                       name(name) {}

Token::Token() = default;

std::string Token::to_string() const {
    char buf[1024];
    sprintf(buf, "%-5d\t%-2d\t%-20s\t%-s", row, column, get_typename(type).c_str(), name.c_str());
    return std::string(buf);
}

std::string Token::get_typename(int type) {
    switch (type) {
        case KEY_WORD:
            return "(关键字)keyword";
        case OPERATOR:
            return "(操作符)operator";
        case DELIMITERS:
            return "(分隔符)delimiter";
        case IDENTIFIER:
            return "(标识符)id";
        case NUMBER:
            return "(数字)  number";
        case STRING:
            return "(字符串)string";
        case CHAR:
            return "(字符)  char";
        case ANNOTATION:
            return "(注解)  annotation";
        default:
            return "(未知)  unknown";
    }
}
