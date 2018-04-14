/**
 * Token 结构
 * 单纯的用来存储数据
 */
#ifndef C_TOKEN_H
#define C_TOKEN_H

#include <string>

class Token {
public:
    int row;
    int column;
    int type;
    std::string name;

    Token(int row, int column, int type, const std::string &name);

    Token();

    std::string to_string() const;

    static std::string get_typename(int type);

    static const int KEY_WORD = 1;
    static const int OPERATOR = 2;
    static const int DELIMITERS = 3;
    static const int IDENTIFIER = 4;
    static const int NUMBER = 5;
    static const int STRING = 6;
    static const int CHAR = 7;
    static const int ANNOTATION = 8;
};

class Symbol {
public:
    int code = -1;
    std::string name;

    Symbol() = default;

    Symbol(const std::string &s, int code) : code(code), name(name) {}

    bool operator<(const Symbol &s) const {
        return !(code == s.code && name == s.name);
    }

    bool operator()(const Symbol &t1, const Symbol &t2) {
        return !(t2.code == t1.code && t2.name == t1.name);
    }

    bool operator==(const Symbol &s) const {
        return name == s.name;
    }
};

#endif //C_TOKEN_H
