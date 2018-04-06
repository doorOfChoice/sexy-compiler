//
// Created by dawndevil on 2018/4/6.
//

#include "Table.h"
#include <fstream>

std::set<std::string> loadMap(const std::string &fname) {
    std::fstream f;
    f.open(fname, std::ios::in);
    char buf[100];
    if (f.is_open()) {
        std::set<std::string> set;
        while (!f.eof()) {
            f.getline(buf, 100);
            set.insert({buf});
        }
        return set;
    }
    f.close();
    return std::set<std::string>();
}

void Table::loadKeyWords(const std::string &fname) {
    this->keyWords = loadMap(fname);
}

void Table::loadOperators(const std::string &fname) {
    this->operators = loadMap(fname);
}

void Table::loadDelimiters(const std::string &fname) {
    this->delimiters = loadMap(fname);
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

const std::vector<Token> &Table::getTokens() {
    return tokens;
}

bool Table::inKey(const std::string &key) {
    return std::find(keyWords.begin(), keyWords.end(), key) != keyWords.end();
}

bool Table::inIdentifier(const std::string &key) {
    return std::find(identifiers.begin(), identifiers.end(), key) != identifiers.end();
}

bool Table::inDelimiter(const std::string &key) {
    return std::find(delimiters.begin(), delimiters.end(), key) != delimiters.end();
}

bool Table::inDelimiter(const char ch) {
    std::string s;
    s.push_back(ch);
    return this->inDelimiter(s);
}

void Table::loadAll() {
    loadKeyWords("keywords.txt");
    loadOperators("operators.txt");
    loadDelimiters("delimiters.txt");
}

void Table::addToken(const Token &token) {
    tokens.push_back(token);
}

void Table::addIdentifier(const std::string &key) {
    identifiers.insert({key});
}

