//
// Created by dawndevil on 2018/4/6.
//

#include "table.h"
#include "string_util.h"
#include <fstream>
#include <iostream>


std::set<std::string> load(const std::string &fname) {
    std::fstream f;
    f.open(fname, std::ios::in);
    char buf[4096];
    if (f.is_open()) {
        std::set<std::string> set;
        while (!f.eof()) {
            f.getline(buf, sizeof(buf) / sizeof(char));
            set.insert({buf});
        }
        return set;
    }
    f.close();
    return std::set<std::string>();
}

void Table::load_keywords(const std::string &fname) {
    this->keyWords = load(fname);
}

void Table::load_operators(const std::string &fname) {
    this->operators = load(fname);
}

void Table::load_delimiters(const std::string &fname) {
    this->delimiters = load(fname);
}

const std::set<std::string> &Table::get_keywords() {
    return keyWords;
}

const std::set<std::string> &Table::get_operators() {
    return operators;
}

const std::set<std::string> &Table::get_delimiters() {
    return delimiters;
}

bool Table::in_key(const std::string &key) {
    return std::find(keyWords.begin(), keyWords.end(), key) != keyWords.end();
}

bool Table::in_delimiter(const std::string &key) {
    return std::find(delimiters.begin(), delimiters.end(), key) != delimiters.end();
}

bool Table::in_operator(const std::string &key) {
    return std::find(operators.begin(), operators.end(), key) != operators.end();
}

void Table::load_all() {
    load_keywords("keywords.txt");
    load_operators("operators.txt");
    load_delimiters("delimiters.txt");
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


