#include <iostream>
#include "lexical.h"
#include "base/string_util.h"

Lexical::Lexical(const Table &table) : table(table) {}

const vector<ErrorInfoException> &Lexical::get_errors() const {
    return errors;
}

const vector<Token> &Lexical::get_tokens() const {
    return tokens;
}

const set<Symbol> &Lexical::get_symbols() const {
    return symbols;
}

/**
 * 词法分析的入口
 * @param lines 需要分析的行段
 */
void Lexical::analyse(vector<shared_ptr<StringLine>> lines) {
    Meta meta;
    for (const auto &line : lines) {
        auto begin = line->get_text().begin();
        auto end = line->get_text().end();
        auto it = begin;
        int lineNumber = line->get_line();
        while (it != end) {
            int column = it - begin + 1;
            meta = Meta(lineNumber, column, it, begin, end);
            //空白字符不判断
            if (sutil::is_blank(*it)) {
                ++it;
                continue;
            }
            if (sutil::is_key(*it, true)) {
                if (analyse_identifier(it, meta)) continue;
            } else if (isdigit(*it) || (*it == '0' && tolower(*it) == 'x') || (*it == '.' && isdigit(*(it + 1)))) {
                if (analyse_number(it, meta))continue;
            } else if (*it == '@') {
                if (analyse_annotation(it, meta))continue;
            } else if (table.in_delimiter(*it)) {
                if (analyse_delimiter(it, meta))continue;
            } else if (table.in_operator(*it)) {
                if (analyse_operator(it, meta))continue;
            } else if (*it == '\'') {
                if (analyse_char(it, meta))continue;
            } else if (*it == '\"') {
                if (analyse_string(it, meta))continue;
            } else {
                error(it, meta, "非法的字符");
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
bool Lexical::analyse_number(string::iterator &it, const Meta &m) {
    int state = 0;
    string buf;
    while (it != m.end) {
        switch (state) {
            case 0: {
                if (isdigit(*it)) {
                    if (*it == '0' && tolower(*(it + 1)) == 'x') {
                        ++it;
                        buf.push_back('0');
                        buf.push_back(*it);
                        state = 8;
                        break;
                    }
                    --it;
                    state = 1;
                } else if (*it == '.') {
                    state = 3;
                    buf.push_back(*it);
                } else
                    return false;

                break;
            }
                //普通数字判断
            case 1: {
                if (isdigit(*it))buf.push_back(*it);
                else if (tolower(*it) == 'f') {
                    buf.push_back(*it);
                    state = 7;
                } else if (tolower(*it) == 'l') {
                    buf.push_back(*it);
                    state = 9;
                } else if (*it == '.') {
                    buf.push_back(*it);
                    state = 2;
                } else if (*it == 'e') {
                    buf.push_back(*it);
                    state = 4;
                } else if (!isalpha(*it)) {
                    --it;
                    state = 10;
                } else {
                    error(it, m, "错误的数字表达方式");
                    return false;
                }

                break;
            }
                //小数判断
            case 2: {
                if (!isdigit(*it)) {
                    error(it, m, "错误的小数");
                    return false;
                }
                --it;
                state = 3;
                break;
            }
                // .4 类型小数
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
                    state = 10;
                } else {
                    error(it, m, "错误的小数");
                    return false;
                }

                break;
            }
                //e计数
            case 4: {
                if (*it == '-' || *it == '+') {
                    buf.push_back(*it);
                    state = 5;
                } else if (isdigit(*it)) {
                    --it;
                    state = 6;
                } else {
                    error(it, m, "错误的科学记数法");
                    return false;
                }
                break;
            }
                //e的指数,含+-
            case 5: {
                if (!isdigit(*it)) {
                    error(it, m, "错误的e指数");
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
                    state = 10;
                } else {
                    error(it, m, "错误的e指数");
                    return false;
                }
                break;
            }
                //处理浮点数符号f
            case 7: {
                if (!isalpha(*it)) {
                    --it;
                    state = 10;
                } else {
                    error(it, m, "错误的f后缀表示");
                    return false;
                }
                break;
            }
            case 8: {
                if (isxdigit(*it))buf.push_back(*it);
                else if (tolower(*(it - 1)) != 'x' && !isalpha(*it)) {
                    --it;
                    state = 10;
                } else {
                    error(it, m, "错误的十六进制表示");
                    return false;
                }
                break;
            }
            case 9: {
                if (!isalpha(*it)) {
                    --it;
                    state = 10;
                } else {
                    error(it, m, "非16进制数字后面不应该跟无关字母");
                    return false;
                }
                break;
            }
            case 10: {
                Token t(m.line, m.column, Token::NUMBER, buf);
                tokens.push_back(t);
                symbols.insert(Symbol(t.name, t.type));
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
bool Lexical::analyse_identifier(string::iterator &it, const Meta &m) {
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
                if (t.type == Token::IDENTIFIER) {
                    symbols.insert(Symbol(t.name, t.type));
                }
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
bool Lexical::analyse_delimiter(string::iterator &it, const Meta &m) {
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
bool Lexical::analyse_operator(string::iterator &it, const Meta &m) {
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
bool Lexical::analyse_char(string::iterator &it, const Meta &m) {
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
                //分析普通字符
            case 2: {
                if (*it != '\'') {
                    error(it, m, "非法的字符结尾");
                    return false;
                }
                state = 6;
                break;
            }
                //分析普通转义字符
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
                    error(it, m, "非法的转义字符");
                    return false;
                }
                break;
            }
                // 分析\uFFFF
            case 4: {
                if (countU++ < 4) {
                    if (isxdigit(*it))buf.push_back(*it);
                    else {
                        error(it, m, "unicode表示法需要4位16进制数");
                        return false;
                    }
                } else {
                    --it;
                    state = 2;
                }
                break;
            }
                //分析 \八进制*3
            case 5: {
                if (countO < 3) {
                    if (sutil::is_octal(*it)) {
                        ++countO;
                        buf.push_back(*it);
                    } else if (countO != 0) {
                        --it;
                        state = 2;
                    } else {
                        error(it, m, "八进制字符需要1-3位八进制数");
                        return false;
                    }
                } else {
                    --it;
                    state = 2;
                }
                break;
            }
            case 6: {
                Token t(m.line, m.column, Token::CHAR, buf);
                tokens.push_back(t);
                symbols.insert(Symbol(t.name, t.type));
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
bool Lexical::analyse_string(string::iterator &it, const Meta &m) {
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
                //分析普通字符
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
                //分析普通转义字符
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
                } else {
                    error(it, m, "非法的转义字符");
                    return false;
                }
                break;
            }
                //分析 \uFFFF
            case 3: {
                if (countU++ < 4) {
                    if (isxdigit(*it))buf.push_back(*it);
                    else {
                        error(it, m, "unicode表示法需要4位16进制数");
                        return false;
                    }
                } else {
                    --it;
                    countU = 0;
                    state = 1;
                }
                break;
            }
                // 分析\八进制*3
            case 4: {
                auto init = [&it, &countO, &state]() -> void {
                    --it;
                    countO = 0;
                    state = 1;
                };
                if (countO++ < 3) {
                    if (sutil::is_octal(*it)) {
                        ++countO;
                        buf.push_back(*it);
                    } else if (countO != 0) {
                        init();
                    } else {
                        error(it, m, "八进制字符需要1-3位八进制数");
                        return false;
                    }
                } else
                    init();

                break;
            }
            case 5: {
                Token t(m.line, m.column, Token::STRING, buf);
                tokens.push_back(t);
                symbols.insert(Symbol(t.name, t.type));
                return true;
            }
        }
        ++it;
    }
    return false;
}

bool Lexical::analyse_annotation(string::iterator &it, const Meta &m) {
    int state = 0;
    string buf;
    while (it != m.end) {
        switch (state) {
            case 0: {
                state = 1;
                break;
            }
            case 1: {
                if (!sutil::is_key(*it, true)) {
                    error(it, m, "注解的不能以非字母开头");
                    return false;
                }
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
                symbols.insert(Symbol(t.name, t.type));
                return true;
            }
        }
        ++it;
    }
    return false;
}

bool Lexical::error(string::iterator &it, const Meta &m, const string &message) {
    errors.emplace_back(m.line, m.column, it - m.begin + 1, message + ":" + string(m.cur_begin, it + 1));
//    while (it != m.end && !sutil::is_blank(*it) && !table.in_delimiter(*it))++it;
    ++it;
    return false;
}


