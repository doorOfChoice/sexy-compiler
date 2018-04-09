#include <iostream>
#include "lexical.h"
#include "base/string_util.h"

lexical::lexical(const Table &table) : table(table) {}

const vector<error_info_exception> &lexical::get_errors() const {
    return errors;
}

const vector<Token> &lexical::get_tokens() const {
    return tokens;
}

const set<string> &lexical::get_identifiers() const {
    return identifiers;
}

/**
 * 词法分析的入口
 * @param lines 需要分析的行段
 */
void lexical::analyse(vector<shared_ptr<StringLine>> lines) {
    for (const auto &line : lines) {
        auto begin = line->get_text().begin();
        auto end = line->get_text().end();
        auto it = line->get_text().begin();
        int lineNumber = line->get_line();
        while (it != end) {
            int column = it - begin + 1;
            Meta meta = Meta(lineNumber, column, end);
            if (StringUtil::is_blank(*it))++it;
            else if (analyse_delimiter(it, meta));
            else if (analyse_identifier(it, meta));
            else if (analyse_number(it, meta));
            else if (analyse_operator(it, meta));
            else if (analyse_char(it, meta));
            else if (analyse_string(it, meta));
            else {
                errors.emplace_back(lineNumber, it - begin + 1, string("Unknow Char: ") + *it);
                return;
            }
        }
    }
}

/**
 * 分析科学记数法1.2e-100
 * @param it
 * @param m
 * @return
 */
bool lexical::analyse_number(string::iterator &it, const Meta &m) {
    int state = 1;
    string buf;
    while (it != m.end) {
        switch (state) {
            case 1: {
                if (!isdigit(*it))
                    return false;
                state = 2;
                --it;
                break;
            }
            case 2: {
                if (isdigit(*it))buf.push_back(*it);
                else if (tolower(*it) == 'f') {
                    buf.push_back(*it);
                    state = 9;
                } else if (tolower(*it) == 'l') {
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
                } else
                    return false;

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
                } else if (tolower(*it) == 'l') {
                    buf.push_back(*it);
                    state = 10;
                } else if (*it == 'e') {
                    buf.push_back(*it);
                    state = 5;
                } else if (!isalpha(*it)) {
                    --it;
                    state = 8;
                } else
                    return false;

                break;
            }
            case 5: {
                if (*it == '-' || *it == '+') {
                    buf.push_back(*it);
                    state = 6;
                } else if (isdigit(*it)) {
                    --it;
                    state = 7;
                } else
                    return false;
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
bool lexical::analyse_identifier(string::iterator &it, const Meta &m) {
    int state = 0;
    string buf;
    while (it != m.end) {
        switch (state) {
            case 0: {
                if (!StringUtil::is_key(*it, true))
                    return false;
                state = 1;
                --it;
                break;
            }
            case 1: {
                if (StringUtil::is_key(*it))buf.push_back(*it);
                else {
                    --it;
                    state = 2;
                }
                break;
            }
            case 2: {
                Token t(m.line, m.column, table.in_key(buf) ? Token::KEY_WORD : Token::IDENTIFIER, buf);
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
bool lexical::analyse_delimiter(string::iterator &it, const Meta &m) {
    string buf;
    buf.push_back(*it);
    if (table.in_delimiter(buf)) {
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
bool lexical::analyse_operator(string::iterator &it, const Meta &m) {
    int state = 0;
    string buf;
    buf.push_back(*it);
    while (it != m.end) {
        switch (state) {
            case 0: {
                if (!table.in_operator(buf))
                    return false;
                state = 1;
                break;
            }
            case 1: {
                buf.push_back(*it);
                if (!table.in_operator(buf)) {
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

bool lexical::analyse_char(string::iterator &it, const Meta &m) {
    int state = 0;
    string buf;
    auto isSpecial = [](char ch) -> bool { return ch == '\\' || ch == '"' || ch == '\''; };
    int countU = 0;
    while (it != m.end) {
        switch (state) {
            case 0: {
                if (*it != '\'')
                    return false;
                state = 1;
                break;
            }
            case 1: {
                if (*it == '\\')
                    state = 4;
                else
                    state = 2;
                buf.push_back(*it);
                break;
            }
            case 2: {
                if (*it != '\'')
                    return false;
                state = 3;
                break;
            }
            case 3: {
                Token t(m.line, m.column, Token::CHAR, buf);
                tokens.push_back(t);
                return true;
            }
            case 4: {
                buf.push_back(*it);
                if (isSpecial(*it)) {
                    state = 2;
                } else if (*it == 'u') {
                    state = 5;
                } else {
                    return false;
                }
                break;
            }
            case 5: {
                if (countU++ < 4) {
                    if (isdigit(*it))buf.push_back(*it);
                    else return false;
                } else {
                    --it;
                    state = 2;
                }
            }
        }
        ++it;
    }
    return false;
}

bool lexical::analyse_string(string::iterator &it, const Meta &m) {
    int state = 0;
    string buf;

    //判断是否是需要转义的字符
    auto isSpecial = [](char ch) -> bool { return ch == '\\' || ch == '"' || ch == '\''; };
    int countU = 0;
    while (it != m.end) {
        switch (state) {
            case 0: {
                if (*it != '"')
                    return false;
                state = 1;
                break;
            }
            case 1: {
                if (*it == '\\') {
                    state = 2;
                    buf.push_back(*it);
                } else if (*it == '"')
                    state = 4;
                else
                    buf.push_back(*it);

                break;
            }
            case 2: {
                buf.push_back(*it);
                if (isSpecial(*it)) {
                    state = 1;
                } else if (*it == 'u') {
                    state = 3;
                } else
                    return false;
                break;
            }
            case 3: {
                if (countU++ < 4) {
                    if (isdigit(*it))buf.push_back(*it);
                    else return false;
                } else {
                    countU = 0;
                    state = 1;
                    --it;
                }
                break;
            }
            case 4: {
                Token t(m.line, m.column, Token::STRING, buf);
                tokens.push_back(t);
                return true;
            }
        }
        ++it;
    }
    return false;
}


