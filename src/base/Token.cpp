//
// Created by dawndevil on 2018/4/7.
//

#include "Token.h"
#include <ostream>

int Token::getRow() const {
    return row;
}

void Token::setRow(int row) {
    Token::row = row;
}

int Token::getColumn() const {
    return column;
}

void Token::setColumn(int column) {
    Token::column = column;
}

int Token::getType() const {
    return type;
}

void Token::setType(int type) {
    Token::type = type;
}

const std::string &Token::getName() const {
    return name;
}

void Token::setName(const std::string &name) {
    Token::name = name;
}

Token::Token(int row, int column, int type, const std::string &name) : row(row), column(column), type(type),
                                                                       name(name) {}

Token::Token() = default;

std::string Token::to_string()const {
    char buf[1024];
    sprintf(buf, "(%d, %d, %s, %s)", row, column, getTypeName(type).c_str(), name.c_str());
    return std::string(buf);
}

std::string Token::getTypeName(int type) {
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
        default:
            return "unknown";
    }
}
