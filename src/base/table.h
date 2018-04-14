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
#include "token.h"
#include "string_line.h"
#include "../plugins/json.hpp"

using json = nlohmann::json;

class Table {
public:
    void load_all(const std::string &fname);

    const std::set<Symbol> &get_keywords();

    const std::set<Symbol> &get_operators();

    const std::set<Symbol> &get_delimiters();

    const std::set<std::string> &get_escape_chars();

    bool in_key(const std::string &key);

    bool in_delimiter(const std::string &key);

    bool in_operator(const std::string &key);

    bool in_delimiter(const char &ch);

    bool in_operator(const char &ch);

    bool in_escape_chars(const char &ch);

private:
    std::set<Symbol> keyWords;
    std::set<Symbol> operators;
    std::set<Symbol> delimiters;
    std::set<std::string> escape_chars;

    void load_keywords(const json &);

    void load_operators(const json &);

    void load_delimiters(const json &);

    void load_escape_chars(const json &);
};


#endif //C_TOKENTABLE_H
