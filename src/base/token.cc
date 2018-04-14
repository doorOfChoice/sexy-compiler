#include "token.h"
#include <ostream>

namespace token {


    Token::Token(int row, int column, int type, const std::string &name) : row(row), column(column), type(type),
                                                                           name(name) {}

    Token::Token() = default;

    std::string Token::to_string() const {
        char buf[1024];
        sprintf(buf, "%-5d\t%-2d\t%-20s\t%-s", row, column, get_typename(type).c_str(), name.c_str());
        return std::string(buf);
    }


    std::string Token::get_typename(int type) {
        auto range = [](int x, int l, int r)->bool {
            return x >= l && x <= r;
        };
        if(range(type, 1, 50)) return "(关键字)keyword";
        else if(range(type, 51, 78)) return "(操作符)operator";
        else if(range(type, 79, 88)) return "(分隔符)delimiter";

        switch (type) {
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
}