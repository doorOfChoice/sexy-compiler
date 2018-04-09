//
// Created by dawndevil on 2018/4/8.
//

#ifndef C_LEXICAL_H
#define C_LEXICAL_H


#include "base/table.h"

using namespace std;

struct Meta {
    int line;
    int column;
    string::iterator &end;

    Meta(int line, int column, string::iterator &end) : line(
            line), column(column), end(end) {}
};

class lexical {
public:
    lexical() = default;

    lexical(const Table &table);

    /**
    * 生成token表
    * @return
    */
    void analyse(vector<shared_ptr<StringLine>>);

    const vector<Token> &get_tokens() const;

    const set<string> &get_identifiers() const;

    const vector<ErrorInfoException> &get_errors() const;

private:
    Table table;
    vector<Token> tokens;
    vector<ErrorInfoException> errors;
    set<string> identifiers;

    bool analyse_number(string::iterator &it, const Meta &m);

    bool analyse_identifier(string::iterator &it, const Meta &m);

    bool analyse_delimiter(string::iterator &it, const Meta &m);

    bool analyse_operator(string::iterator &it, const Meta &m);

    bool analyse_char(string::iterator &it, const Meta &m);

    bool analyse_string(string::iterator &it, const Meta &m);
};

#endif //C_LEXICAL_H
