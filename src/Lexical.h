//
// Created by dawndevil on 2018/4/8.
//

#ifndef C_LEXICAL_H
#define C_LEXICAL_H


#include "base/Table.h"

using namespace std;

struct Meta {
    int line;
    int column;
    string::iterator &end;

    Meta(int line, int column, string::iterator &end) : line(
            line), column(column), end(end) {}
};

class Lexical {
public:
    Lexical() = default;

    Lexical(const Table &table);

    /**
    * 生成token表
    * @return
    */
    void analyseLines(vector<shared_ptr<StringLine>>);

    const vector<Token> &getTokens() const;

    const set<string> &getIdentifiers() const;

    const vector<ErrorInfoException> &getErrors() const;

private:
    Table table;
    vector<Token> tokens;
    vector<ErrorInfoException> errors;
    set<string> identifiers;

    bool analyseNumber(string::iterator &it, const Meta &m);

    bool analyseIdentifier(string::iterator &it, const Meta &m);

    bool analyseDelimiter(string::iterator &it, const Meta &m);

    bool analyseOperator(string::iterator &it, const Meta &m);

    bool isSuffix(char ch);
};

#endif //C_LEXICAL_H
