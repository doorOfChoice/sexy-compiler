/**
 *  词法表
 *  用来存储非动态存储的词，比如关键字、操作符、分隔符
 */
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

    bool inKey(const std::string &key);

    bool inDelimiter(const std::string &key);

    bool inOperator(const std::string &key);

private:
    std::set<std::string> keyWords;
    std::set<std::string> operators;
    std::set<std::string> delimiters;
};


#endif //C_TOKENTABLE_H
