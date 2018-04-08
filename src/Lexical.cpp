//
// Created by dawndevil on 2018/4/8.
//

#include <iostream>
#include "Lexical.h"
#include "base/StringUtil.h"

Lexical::Lexical(const Table &table) : table(table) {}

void Lexical::analyseLines(vector<shared_ptr<StringLine>> lines) {
    int lineNumber = 0;
    for (const auto &line : lines) {
        auto begin = line->getText().begin();
        auto end = line->getText().end();
        auto it = line->getText().begin();
        lineNumber = line->getLine();
        while (it != end) {
            int column = it - begin + 1;
            if (StringUtil::isBlank(*it))++it;
            else if (analyseDelimiter(it, Meta(lineNumber, column, end)));
            else if (analyseIdentifier(it, Meta(lineNumber, column, end)));
            else if (analyseNumber(it, Meta(lineNumber, column, end)));
            else if (analyseOperator(it, Meta(lineNumber, column, end)));
            else {
                errors.emplace_back(lineNumber, it - begin + 1, string("Unknow Char: ") + *it);
                return;
            }
        }

    }
}

const vector<Token> &Lexical::getTokens() const {
    return tokens;
}

const set<string> &Lexical::getIdentifiers() const {
    return identifiers;
}

/**
 * 分析科学记数法1.2e-100
 * @param it
 * @param m
 * @return
 */
bool Lexical::analyseNumber(string::iterator &it, const Meta &m) {
    int state = 1;
    string buf;
    while (it != m.end) {
        switch (state) {
            case 1: {
                if (isdigit(*it)) {
                    state = 2;
                    --it;
                } else
                    return false;
                break;
            }
            case 2: {
                if (isdigit(*it))buf.push_back(*it);
                else if (tolower(*it) == 'f') {
                    buf.push_back(*it);
                    state = 9;
                }else if(tolower(*it) == 'l') {
                    buf.push_back(*it);
                    state = 10;
                } else if (*it == '.') {
                    buf.push_back(*it);
                    state = 3;
                } else if (*it == 'e') {
                    buf.push_back(*it);
                    state = 5;
                } else if (!isalpha(*it)) {
                    --it;
                    state = 8;
                } else {
                    --it;
                    return false;
                }
                break;
            }
            case 3: {
                if (!isdigit(*it)) {
                    return false;
                }
                --it;
                state = 4;
                break;
            }
            case 4: {
                if (isdigit(*it))buf.push_back(*it);
                else if (tolower(*it) == 'f') {
                    buf.push_back(*it);
                    state = 9;
                } else if(tolower(*it) == 'l') {
                    buf.push_back(*it);
                    state = 10;
                } else if (*it == 'e') {
                    buf.push_back(*it);
                    state = 5;
                } else if (!isalpha(*it)) {
                    --it;
                    state = 8;
                } else {
                    return false;
                }
                break;
            }
            case 5: {
                if (*it == '-') {
                    buf.push_back(*it);
                    state = 6;
                } else if (isdigit(*it)) {
                    --it;
                    state = 7;
                } else {
                    return false;
                }
                break;
            }
            case 6: {
                if (!isdigit(*it)) {
                    return false;
                }
                --it;
                state = 7;
                break;
            }
            case 7: {
                if (isdigit(*it)) buf.push_back(*it);
                else if (!isalpha(*it)) {
                    --it;
                    state = 8;
                } else
                    return false;
                break;
            }
                //处理浮点数符号f
            case 9: {
                if (!isalpha(*it)) {
                    --it;
                    state = 8;
                } else
                    return false;
                break;
            }
            case 10: {
                if (!isalpha(*it)) {
                    --it;
                    state = 8;
                } else
                    return false;
                break;
            }
            case 8: {
                Token t(m.line, m.column, Token::NUMBER, buf);
                tokens.push_back(t);
                return true;
            }
        }
        ++it;
    }
    return false;
}

/**
 * 分析标识符
 * @param it
 * @param m
 * @return
 */
bool Lexical::analyseIdentifier(string::iterator &it, const Meta &m) {
    int state = 0;
    string buf;
    while (it != m.end) {
        switch (state) {
            case 0: {
                if (!StringUtil::isKey(*it, true))
                    return false;
                state = 1;
                --it;
                break;
            }
            case 1: {
                if (StringUtil::isKey(*it))buf.push_back(*it);
                else {
                    --it;
                    state = 2;
                }
                break;
            }
            case 2: {
                Token t(m.line, m.column, table.inKey(buf) ? Token::KEY_WORD : Token::IDENTIFIER, buf);
                tokens.push_back(t);
                return true;
            }
        }
        ++it;
    }
    return false;
}

/**
 * 分析分隔符
 * @param it
 * @param m
 * @return
 */
bool Lexical::analyseDelimiter(string::iterator &it, const Meta &m) {
    string buf;
    buf.push_back(*it);
    if (table.inDelimiter(buf)) {
        Token t(m.line, m.column, Token::DELIMITERS, buf);
        tokens.push_back(t);
        ++it;
        return true;
    }
    return false;
}

/**
 * 分割运算符
 * @param it
 * @param m
 * @return
 */
bool Lexical::analyseOperator(string::iterator &it, const Meta &m) {
    int state = 0;
    string buf;
    buf.push_back(*it);
    while (it != m.end) {
        switch (state) {
            case 0: {
                if (!table.inOperator(buf))
                    return false;
                state = 1;
                break;
            }
            case 1: {
                buf.push_back(*it);
                if (!table.inOperator(buf)) {
                    state = 2;
                    --it;
                    buf.pop_back();
                }
                break;
            }
            case 2: {
                Token t(m.line, m.column, Token::OPERATOR, buf);
                tokens.push_back(t);
                return true;
            }
        }
        ++it;
    }
    return false;
}

const vector<ErrorInfoException> &Lexical::getErrors() const {
    return errors;
}

bool Lexical::isSuffix(char ch) {
    return table.inOperator(ch) || table.inDelimiter(ch) || StringUtil::isBlank(ch);
}



