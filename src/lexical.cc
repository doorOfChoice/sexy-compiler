#include <iostream>
#include "lexical.h"
#include "base/string_util.h"

lexical::lexical(const Table &table) : table(table) {}

const vector<ErrorInfoException> &lexical::get_errors() const {
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
    Meta meta;
    int error_type = -1;
    int colum_end = 0;
    bool error_happen = false;

    for (const auto &line : lines) {
        auto begin = line->get_text().begin();
        auto end = line->get_text().end();
        auto it = begin;
        int lineNumber = line->get_line();
        while (it != end) {
            int column = it - begin + 1;
            meta = Meta(lineNumber, column, end);
            //空白字符不判断
            if (sutil::is_blank(*it)) {
                ++it;
                continue;
            }
            if (sutil::is_key(*it, true)) {
                if (analyse_identifier(it, meta)) continue;
                error_type = Token::IDENTIFIER;
                error_happen = true;
                break;
            } else if (isdigit(*it) || (*it == '.' && isdigit(*(it + 1)))) {
                if (analyse_number(it, meta))continue;
                error_type = Token::NUMBER;
                error_happen = true;
                break;
            } else if (*it == '@') {
                if (analyse_annotation(it, meta))continue;
                error_type = Token::ANNOTATION;
                error_happen = true;
                break;
            } else if (table.in_delimiter(*it)) {
                if (analyse_delimiter(it, meta))continue;
                error_type = Token::DELIMITERS;
                error_happen = true;
                break;
            } else if (table.in_operator(*it)) {
                if (analyse_operator(it, meta))continue;
                error_type = Token::OPERATOR;
                error_happen = true;
                break;
            } else if (*it == '\'') {
                if (analyse_char(it, meta))continue;
                error_type = Token::CHAR;
                error_happen = true;
                break;
            } else if (*it == '\"') {
                if (analyse_string(it, meta))continue;
                error_type = Token::STRING;
                error_happen = true;
                break;
            } else {
                error_happen = true;
                break;
            }
        }
        if (error_happen) {
            colum_end = it - begin + 1;
            break;
        }
    }

    if (error_happen) {
        errors.emplace_back(meta.line, meta.column, colum_end,
                                            string(Token::get_typename(error_type)) + "解析出现错误");
    }
}

/**
 * 分析科学记数法1.2e-100
 * @param it
 * @param m
 * @return
 */
bool lexical::analyse_number(string::iterator &it, const Meta &m) {
    int state = 0;
    string buf;
    while (it != m.end) {
        switch (state) {
            case 0: {
                if (isdigit(*it)) {
                    state = 1;
                    --it;
                } else if (*it == '.') {
                    state = 3;
                    buf.push_back(*it);
                }
                break;
            }
            case 1: {
                if (isdigit(*it))buf.push_back(*it);
                else if (tolower(*it) == 'f') {
                    buf.push_back(*it);
                    state = 7;
                } else if (tolower(*it) == 'l') {
                    buf.push_back(*it);
                    state = 8;
                } else if (*it == '.') {
                    buf.push_back(*it);
                    state = 2;
                } else if (*it == 'e') {
                    buf.push_back(*it);
                    state = 4;
                } else if (!isalpha(*it)) {
                    --it;
                    state = 9;
                } else
                    return false;

                break;
            }
            case 2: {
                if (!isdigit(*it)) {
                    return false;
                }
                --it;
                state = 3;
                break;
            }
            case 3: {
                if (isdigit(*it))buf.push_back(*it);
                else if (tolower(*it) == 'f') {
                    buf.push_back(*it);
                    state = 7;
                } else if (*it == 'e') {
                    buf.push_back(*it);
                    state = 4;
                } else if (!isalpha(*it)) {
                    --it;
                    state = 9;
                } else
                    return false;

                break;
            }
            case 4: {
                if (*it == '-' || *it == '+') {
                    buf.push_back(*it);
                    state = 5;
                } else if (isdigit(*it)) {
                    --it;
                    state = 6;
                } else
                    return false;
                break;
            }
            case 5: {
                if (!isdigit(*it)) {
                    return false;
                }
                --it;
                state = 6;
                break;
            }
            case 6: {
                if (isdigit(*it)) buf.push_back(*it);
                else if (!isalpha(*it)) {
                    --it;
                    state = 9;
                } else
                    return false;
                break;
            }
                //处理浮点数符号f
            case 7: {
                if (!isalpha(*it)) {
                    --it;
                    state = 9;
                } else
                    return false;
                break;
            }
            case 8: {
                if (!isalpha(*it)) {
                    --it;
                    state = 9;
                } else
                    return false;
                break;
            }
            case 9: {
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
                --it;
                state = 1;
                break;
            }
            case 1: {
                if (sutil::is_key(*it))buf.push_back(*it);
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
    while (it != m.end) {
        switch (state) {
            case 0: {
                state = 1;
                buf.push_back(*it);
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

/**
 * 分析单个字符
 * @param it
 * @param m
 * @return
 */
bool lexical::analyse_char(string::iterator &it, const Meta &m) {
    int state = 0;
    string buf;
    int countU = 0;
    int countO = 0;
    while (it != m.end) {
        switch (state) {
            case 0: {
                state = 1;
                break;
            }
            case 1: {
                if (*it == '\\')
                    state = 3;
                else
                    state = 2;
                buf.push_back(*it);
                break;
            }
            case 2: {
                if (*it != '\'')
                    return false;
                state = 6;
                break;
            }
            case 3: {
                buf.push_back(*it);
                if (table.in_escape_chars(*it)) {
                    state = 2;
                } else if (*it == 'u') {
                    state = 4;
                } else if (sutil::is_octal(*it)) {
                    state = 5;
                    --it;
                    buf.pop_back();
                } else {
                    return false;
                }
                break;
            }
            case 4: {
                if (countU++ < 4) {
                    if (isxdigit(*it))buf.push_back(*it);
                    else return false;
                } else {
                    --it;
                    state = 2;
                }
                break;
            }
            case 5: {
                if (countO++ < 3) {
                    if (sutil::is_octal(*it))buf.push_back(*it);
                    else return false;
                } else {
                    --it;
                    state = 2;
                }
                break;
            }
            case 6: {
                Token t(m.line, m.column, Token::CHAR, buf);
                tokens.push_back(t);
                return true;
            }
        }
        ++it;
    }
    return false;
}

/**
 * 分析字符串
 * @param it
 * @param m
 * @return
 */
bool lexical::analyse_string(string::iterator &it, const Meta &m) {
    int state = 0;
    string buf;

    //判断是否是需要转义的字符
    int countU = 0;
    int countO = 0;
    while (it != m.end) {
        switch (state) {
            case 0: {
                state = 1;
                break;
            }
            case 1: {
                if (*it == '\\') {
                    state = 2;
                    buf.push_back(*it);
                } else if (*it == '"')
                    state = 5;
                else
                    buf.push_back(*it);

                break;
            }
            case 2: {
                buf.push_back(*it);
                if (table.in_escape_chars(*it)) {
                    state = 1;
                } else if (*it == 'u') {
                    state = 3;
                } else if (sutil::is_octal(*it)) {
                    --it;
                    buf.pop_back();
                    state = 4;
                } else
                    return false;
                break;
            }
            case 3: {
                if (countU++ < 4) {
                    if (isxdigit(*it))buf.push_back(*it);
                    else return false;
                } else {
                    countU = 0;
                    state = 1;
                    --it;
                }
                break;
            }
            case 4: {
                if (countO++ < 3) {
                    if (sutil::is_octal(*it))buf.push_back(*it);
                    else return false;
                } else {
                    --it;
                    countO = 0;
                    state = 1;
                }
                break;
            }
            case 5: {
                Token t(m.line, m.column, Token::STRING, buf);
                tokens.push_back(t);
                return true;
            }
        }
        ++it;
    }
    return false;
}

bool lexical::analyse_annotation(string::iterator &it, const Meta &m) {
    int state = 0;
    string buf;
    while (it != m.end) {
        switch (state) {
            case 0: {
                state = 1;
                break;
            }
            case 1: {
                if (!sutil::is_key(*it, true))
                    return false;
                --it;
                state = 2;
                break;
            }
            case 2: {
                if (sutil::is_key(*it))buf.push_back(*it);
                else {
                    --it;
                    state = 3;
                }
                break;
            }
            case 3: {
                Token t(m.line, m.column, Token::ANNOTATION, buf);
                tokens.push_back(t);
                return true;
            }
        }
        ++it;
    }
    return false;
}


