//
// Created by dawndevil on 2018/4/8.
//

#include "Lexical.h"
#include "base/StringUtil.h"



Lexical::Lexical(const Table &table) : table(table) {}

void Lexical::analyseLines(std::vector<std::shared_ptr<StringLine>> lines) {
    for (const auto &line : lines) {
        std::string buf;
        auto begin = line->getText().begin();
        for (auto it = begin; it != line->getText().end(); it++) {
            int column = it - begin + 1;
            if (!isblank(*it)) {
                buf.push_back(*it);
                //判断关键字
                if (StringUtil::isKey(*it, true)) {
                    while (StringUtil::isKey(*(++it))) {
                        buf.push_back(*it);
                    }
                    int type = table.inKey(buf) ? Token::KEY_WORD : Token::IDENTIFIER;
                    identifiers.insert({buf});
                    Token t(line->getLine(), column, type, buf);
                    tokens.push_back(t);
                    --it;
                    //判断分隔符
                } else if (table.inDelimiter(buf)) {
                    Token t(line->getLine(), column, Token::DELIMITERS, buf);
                    tokens.push_back(t);
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
                    tokens.push_back(t);
                    //判断运算符
                } else if (table.inOperator(buf)) {
                    while (table.inOperator(buf)) {
                        buf.push_back(*(++it));
                    }
                    buf.pop_back();
                    --it;
                    Token t(line->getLine(), column, Token::OPERATOR, buf);
                    tokens.push_back(t);
                    //判断未知字符
                } else {
                    Token t(line->getLine(), column, -1, buf);
                    tokens.push_back(t);
                }
            }
            buf.clear();
        }
    }
}

const std::vector<Token> &Lexical::getTokens() const {
    return tokens;
}

const std::set<std::string> &Lexical::getIdentifiers() const {
    return identifiers;
}

void Lexical::analyseNumber(std::string::iterator it) {

}

void Lexical::analyseIdentifier(std::string::iterator it) {

}

void Lexical::analyseDelimiter(std::string::iterator it) {

}

void Lexical::analyseOperator(std::string::iterator it) {

}


