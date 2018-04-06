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

std::ostream &operator<<(std::ostream &os, const Token &m) {
    os << "row: " << m.row << ", column: " << m.column << ", type: " << Token::getTypeName(m.type) << ", name: " << m.name;
    return os;
}

std::string Token::getTypeName(int type) {
    switch (type) {
        case 1:
            return "key word";
        case 2:
            return "operator";
        case 3:
            return "delimiter";
        case 4:
            return "identifier";
        default:
            return "unknown";
    }
}
