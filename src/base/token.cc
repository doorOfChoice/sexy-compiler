#include "token.h"
#include <ostream>


Token::Token(int row, int column, int type, const std::string &name) : row(row), column(column), type(type),
                                                                       name(name) {}

Token::Token() = default;

std::string Token::to_string() const {
    char buf[1024];
    sprintf(buf, "%-5d\t%-2d\t%-10s\t%-s", row, column, get_typename(type).c_str(), name.c_str());
    return std::string(buf);
}

std::string Token::get_typename(int type) {
    switch (type) {
        case KEY_WORD:
            return "keyword";
        case OPERATOR:
            return "operator";
        case DELIMITERS:
            return "delimiter";
        case IDENTIFIER:
            return "id";
        case NUMBER:
            return "number";
        case STRING:
            return "string";
        case CHAR:
            return "char";
        case ANNOTATION:
            return "annotation";
        default:
            return "unknown";
    }
}
