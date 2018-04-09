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

class Table {
public:
    void load_keywords(const std::string &fname);

    void load_operators(const std::string &fname);

    void load_delimiters(const std::string &fname);

    void load_escape_chars(const std::string &fname);

    void load_all();

    const std::set<std::string> &get_keywords();

    const std::set<std::string> &get_operators();

    const std::set<std::string> &get_delimiters();

    const std::set<std::string> &get_escape_chars();

    bool in_key(const std::string &key);

    bool in_delimiter(const std::string &key);

    bool in_operator(const std::string &key);

    bool in_delimiter(const char &ch);

    bool in_operator(const char &ch);

    bool in_escape_chars(const char &ch);
private:
    std::set<std::string> keyWords;
    std::set<std::string> operators;
    std::set<std::string> delimiters;
    std::set<std::string> escape_chars;
};


#endif //C_TOKENTABLE_H
