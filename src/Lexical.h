//
// Created by dawndevil on 2018/4/8.
//

#ifndef C_LEXICAL_H
#define C_LEXICAL_H


#include "base/Table.h"

class Lexical {
public:
    Lexical() = default;
    Lexical(const Table &table);

    /**
    * 生成token表
    * @return
    */
    void analyseLines(std::vector<std::shared_ptr<StringLine>>);

    const std::vector<Token> &getTokens() const;
    const std::set<std::string> &getIdentifiers() const;
private:
    Table table;
    std::vector<Token> tokens;
    std::set<std::string> identifiers;

    void analyseNumber(std::string::iterator it);
    void analyseIdentifier(std::string::iterator it);
    void analyseDelimiter(std::string::iterator it);
    void analyseOperator(std::string::iterator it);
};


#endif //C_LEXICAL_H
