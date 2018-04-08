//
// Created by dawndevil on 2018/4/6.
//

#include "Table.h"
#include "StringUtil.h"
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

void Table::loadKeyWords(const std::string &fname) {
    this->keyWords = load(fname);
}

void Table::loadOperators(const std::string &fname) {
    this->operators = load(fname);
}

void Table::loadDelimiters(const std::string &fname) {
    this->delimiters = load(fname);
}

const std::set<std::string> &Table::getKeyWords() {
    return keyWords;
}

const std::set<std::string> &Table::getOperators() {
    return operators;
}

const std::set<std::string> &Table::getDelimiters() {
    return delimiters;
}

bool Table::inKey(const std::string &key) {
    return std::find(keyWords.begin(), keyWords.end(), key) != keyWords.end();
}

bool Table::inDelimiter(const std::string &key) {
    return std::find(delimiters.begin(), delimiters.end(), key) != delimiters.end();
}

bool Table::inOperator(const std::string &key) {
    return std::find(operators.begin(), operators.end(), key) != operators.end();
}

void Table::loadAll() {
    loadKeyWords("keywords.txt");
    loadOperators("operators.txt");
    loadDelimiters("delimiters.txt");
}


