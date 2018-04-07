/**
 * Token 结构
 * 单纯的用来存储数据
 */
#ifndef C_TOKEN_H
#define C_TOKEN_H

#include <string>

class Token {
public:
    Token(int row, int column, int type, const std::string &name);

    Token();

    int getRow() const;

    void setRow(int row);

    int getColumn() const;

    void setColumn(int column);

    int getType() const;

    void setType(int type);

    const std::string &getName() const;

    void setName(const std::string &name);

    std::string to_string()const;

    static const int KEY_WORD = 1;
    static const int OPERATOR = 2;
    static const int DELIMITERS = 3;
    static const int IDENTIFIER = 4;
    static const int NUMBER = 5;
    static std::string getTypeName(int type);

private:
    int row;
    int column;
    int type;
    std::string name;
};


#endif //C_TOKEN_H
