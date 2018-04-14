//
// Created by dawndevil on 2018/4/8.
//

#ifndef C_LEXICAL_H
#define C_LEXICAL_H


#include <sstream>
#include <utility>
#include "base/table.h"

using namespace std;

/**
 * 元数据结构
 * 单纯用来存储 行、列、迭代器终点
 */
struct Meta {
    int line;
    int column;
    string::iterator cur_begin;
    string::iterator begin;
    string::iterator end;

    Meta() {}

    Meta(int line, int column, string::iterator &cur_begin, string::iterator &begin, string::iterator &end) : line(
            line), column(column), end(end), begin(begin), cur_begin(cur_begin) {}
};


class Lexical {
public:
    Lexical() = default;

    Lexical(const Table &table);

    /**
    * 生成token表
    * @return
    */
    void analyse(vector<shared_ptr<StringLine>>);

    const vector<Token> &get_tokens() const;

    const set<Symbol> &get_symbols() const;

    const vector<ErrorInfoException> &get_errors() const;

private:
    Table table;
    vector<Token> tokens;
    vector<ErrorInfoException> errors;
    set<Symbol> symbols;

    /**
     * 分析数字常量
     * 详细图可以参看 https://github.com/doorOfChoice/sexy-compiler/blob/master/resource/number.png
     * @param it
     * @param m
     * @return
     */
    bool analyse_number(string::iterator &it, const Meta &m);

    /**
     * 分析标识符
     * 详细图可以参看 https://github.com/doorOfChoice/sexy-compiler/blob/master/resource/keyword.png
     * @param it
     * @param m
     * @return
     */
    bool analyse_identifier(string::iterator &it, const Meta &m);

    /**
     * 分析分隔符
     * @param it
     * @param m
     * @return
     */
    bool analyse_delimiter(string::iterator &it, const Meta &m);

    /**
     * 分析操作符
     * @param it
     * @param m
     * @return
     */
    bool analyse_operator(string::iterator &it, const Meta &m);

    /**
     * 分析单个字符
     * 详细图可以参看 https://github.com/doorOfChoice/sexy-compiler/blob/master/resource/char.png
     * @param it
     * @param m
     * @return
     */
    bool analyse_char(string::iterator &it, const Meta &m);

    /**
     * 分析字符串
     * 详细图可以参看 https://github.com/doorOfChoice/sexy-compiler/blob/master/resource/string.png
     * @param it
     * @param m
     * @return
     */
    bool analyse_string(string::iterator &it, const Meta &m);

    /**
     * 识别注解
     * @param it
     * @param m
     * @return
     */
    bool analyse_annotation(string::iterator &it, const Meta &m);

    bool error(string::iterator &it, const Meta &m, const string &message);
};

#endif //C_LEXICAL_H
