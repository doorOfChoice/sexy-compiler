#include<iostream>
#include <fstream>
#include <memory>
#include<vector>
#include "StringLine.h"
#include "Table.h"

using namespace std;

/**
 * 从文件中读取数据
 * @param fname 文件名
 * @return 可自动回收的字符串指针
 */
shared_ptr<string> readCode(const string &fname) {
    fstream f;
    f.open(fname, ios::in);
    shared_ptr<string> s(new string());
    char buf[100];
    if (f.is_open()) {
        while (!f.eof()) {
            f.getline(buf, sizeof(buf) / sizeof(char));
            s->append(buf);
            s->append("\n");
        }
        f.close();
    } else {
        cout << "File " << fname << " is not exist" << endl;
    }
    return s;
}

bool isKey(char ch, bool isHead = false) {
    if(!isHead)
        return isalpha(ch) || ch == '_' || ch == '$' || isdigit(ch);
    return isalpha(ch) || ch == '_' || ch == '$' ;
}


/**
 * 生成token表
 * @return
 */
void analyseLines(vector<shared_ptr<StringLine>> &lines, Table &table) {
    for (const auto &line : lines) {
        string buf;
        auto begin = line->getText().begin();
        for (auto it = begin; it != line->getText().end(); it++) {
            if (!isblank(*it)) {
                buf.push_back(*it);
                int column = it - begin + 1;
                if (isKey(*it, true)) {
                    while (isKey(*(++it))) {
                        buf.push_back(*it);
                    }
                    int type = table.inKey(buf) ? Token::KEY_WORD : Token::IDENTIFIER;
                    table.addIdentifier(buf);
                    Token t(line->getLine(), column, type, buf);
                    table.addToken(t);
                    buf.clear();
                    --it;
                }else if(table.inDelimiter(*it)) {
                    Token t(line->getLine(), column, Token::DELIMITERS, buf);
                    table.addToken(t);
                    buf.clear();
                }
            }
        }
    }
}


int main() {
    auto lines = StringLine::convertString(readCode("code.java").get());
    Table table;
    table.loadAll();

    analyseLines(lines, table);
    for(auto v : table.getTokens()) {
        cout << v << endl;
    }

}
