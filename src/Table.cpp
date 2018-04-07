//
// Created by dawndevil on 2018/4/6.
//

#include "Table.h"
#include <fstream>
#include <iostream>

bool isKey(char ch, bool isHead = false) {
    if (!isHead)
        return isalpha(ch) || ch == '_' || ch == '$' || isdigit(ch);
    return isalpha(ch) || ch == '_' || ch == '$';
}

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

bool Table::inOperator(const std::string &key) {
    return std::find(operators.begin(), operators.end(), key) != operators.end();
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

void Table::analyseLines(std::vector<std::shared_ptr<StringLine>> lines) {
    for (const auto &line : lines) {
        std::string buf;
        auto begin = line->getText().begin();
        for (auto it = begin; it != line->getText().end(); it++) {
            int column = it - begin + 1;
            if (!isblank(*it)) {
                buf.push_back(*it);
                //判断关键字
                if (isKey(*it, true)) {
                    while (isKey(*(++it))) {
                        buf.push_back(*it);
                    }
                    int type = inKey(buf) ? Token::KEY_WORD : Token::IDENTIFIER;
                    addIdentifier(buf);
                    Token t(line->getLine(), column, type, buf);
                    addToken(t);
                    --it;
                //判断分隔符
                } else if (inDelimiter(buf)) {
                    Token t(line->getLine(), column, Token::DELIMITERS, buf);
                    addToken(t);
                //判断数字
                } else if (isdigit(*it)) {
                    while (isdigit(*(++it))) {
                        buf.push_back(*it);
                    }
                    if (*it == '.' && isdigit(*(it + 1))) {
                        buf.push_back(*it);
                        while (isdigit(*(++it))) {
                            buf.push_back(*it);
                        }
                    }
                    --it;
                    Token t(line->getLine(), column, Token::NUMBER, buf);
                    addToken(t);
                //判断运算符
                }else if(inOperator(buf)) {
                    while(inOperator(buf)) {
                        buf.push_back(*(++it));
                    }
                    buf.pop_back();
                    --it;
                    Token t(line->getLine(), column, Token::OPERATOR, buf);
                    addToken(t);
                //判断未知字符
                } else {
                    Token t(line->getLine(), column, -1, buf);
                    addToken(t);
                }
            }
            buf.clear();
        }
    }
}


