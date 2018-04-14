//
// Created by dawndevil on 2018/4/6.
//

#include "table.h"
#include "string_util.h"
#include <fstream>
#include <iostream>


void Table::load_keywords(const json &j) {
    for(const auto &it : j["keywords"]) {
        keyWords.insert(Symbol(sutil::trim_quotes(it[0]), it[1]));
    }
}

void Table::load_escape_chars(const json &j) {
    for(const auto &it : j["escapes"]) {
        std::string s = it;
        escape_chars.insert(s);
    }
}

void Table::load_operators(const json &j) {
    for(const auto &it : j["operators"]) {
        operators.insert(Symbol(sutil::trim_quotes(it[0]), it[1]));
    }
}

void Table::load_delimiters(const json &j) {
    for(const auto &it : j["delimiters"]) {
        delimiters.insert(Symbol(sutil::trim_quotes(it[0]), it[1]));
    }
}

const std::set<Symbol> &Table::get_keywords() {
    return keyWords;
}

const std::set<Symbol> &Table::get_operators() {
    return operators;
}

const std::set<std::string> &Table::get_escape_chars() {
    return escape_chars;
}

const std::set<Symbol> &Table::get_delimiters() {
    return delimiters;
}

bool Table::in_key(const std::string &key) {
    return std::find(keyWords.begin(), keyWords.end(), Symbol(key, -1)) != keyWords.end();
}

bool Table::in_delimiter(const std::string &key) {
    return std::find(delimiters.begin(), delimiters.end(), Symbol(key, -1)) != delimiters.end();
}

bool Table::in_operator(const std::string &key) {
    return std::find(operators.begin(), operators.end(), Symbol(key, -1)) != operators.end();
}

bool Table::in_escape_chars(const char &ch) {
    return std::find(escape_chars.begin(), escape_chars.end(), std::string() + ch) != escape_chars.end();
}

void Table::load_all(const std::string &fname) {
    std::ifstream f(fname);
    json j;
    f >> j;
    load_delimiters(j);
    load_escape_chars(j);
    load_keywords(j);
    load_operators(j);
}

bool Table::in_delimiter(const char &ch) {
    std::string s;
    s.push_back(ch);
    return in_delimiter(s);
}

bool Table::in_operator(const char &ch) {
    std::string s;
    s.push_back(ch);
    return in_operator(s);
}

