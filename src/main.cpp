#include "plugins/rang.hpp"
#include<iostream>
#include <fstream>
#include <memory>
#include<vector>
#include "base/string_line.h"
#include "base/table.h"
#include "base/error_info_exception.h"
#include "lexical.h"
#include "base/string_util.h"
#include "plugins/crow_all.h"

using namespace std;
using namespace rang;


shared_ptr<Lexical> lexical(const string &s) {
    auto sls = StringLine::convert_string(&s);
    Table table;
    table.load_all("data.json");
    auto lex = make_shared<Lexical>(table);
    lex->analyse(sls.first);
    return lex;
}

crow::response crossrep() {
    auto rep = crow::response(200);
    rep.set_header("Access-Control-Allow-Origin", "*");
    rep.set_header("Access-Control-Allow-Methods", "GET,POST,DELETE,POST,OPTIONS");
    rep.set_header("Access-Control-Allow-Headers", "Access-Control-Allow-Headers,Access-Control-Request-Method,Connection,Host,Origin,User-Agent,Content-Type,Accept-Language,Accept-Encoding,Accept");
    return rep;
}

int main(int argc, char **args) {
    crow::SimpleApp app;


    CROW_ROUTE(app, "/code").methods("POST"_method, "OPTIONS"_method)([](const crow::request &req) {
        if(req.method == crow::HTTPMethod::OPTIONS) {
            cout << "options" << endl;
            return crossrep();
        }
        cout << req.body << endl;
        auto body = crow::json::load(req.body);
        auto lex = lexical(body["code"].s());

        json j;
        j["token"] = lex->get_tokens();
        j["errors"] = lex->get_errors();
        j["symbol"] = lex->get_symbols();
        auto rep = crossrep();
        rep.write(j.dump());
        return rep;
    });

    app.port(8080).multithreaded().run();
    return 0;
}

/*
 * TODO:
 * 1. 需要修复cout输出不完整 √
 * 2. 重新定义一下各种表，只用一种int，太单调
 * 3. 使用状态机重写词法分析器 √
 * 4. 熟练掌握C++使用 √×
 * 5. 遇到异常不进行判断 √
 */