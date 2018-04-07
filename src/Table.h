//
// Created by dawndevil on 2018/4/6.
//

#ifndef C_TOKENTABLE_H
#define C_TOKENTABLE_H

#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include "Token.h"
#include "StringLine.h"

class Table {
public:
    void loadKeyWords(const std::string &fname);

    void loadOperators(const std::string &fname);

    void loadDelimiters(const std::string &fname);

    void loadAll();

    const std::set<std::string> &getKeyWords();

    const std::set<std::string> &getOperators();

    const std::set<std::string> &getDelimiters();

    const std::vector<Token> &getTokens();

    bool inKey(const std::string &key);

    bool inIdentifier(const std::string &key);

    bool inDelimiter(const std::string &key);

    bool inOperator(const std::string &key);

    void addToken(const Token &token);

    void addIdentifier(const std::string &key);

    /**
    * 生成token表
    * @return
    */
    void analyseLines(std::vector<std::shared_ptr<StringLine>>);

private:
    std::set<std::string> keyWords;
    std::set<std::string> operators;
    std::set<std::string> delimiters;
    std::set<std::string> identifiers;
    std::vector<Token> tokens;
};


#endif //C_TOKENTABLE_H
