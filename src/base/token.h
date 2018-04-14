/**
 * Token 结构
 * 单纯的用来存储数据
 */
#ifndef C_TOKEN_H
#define C_TOKEN_H

#include <string>
#include "../plugins/json.hpp"

using json = nlohmann::json;

namespace token {
    class Token {
    public:
        int row;
        int column;
        int type;
        std::string name;

        Token(int row, int column, int type, const std::string &name);

        Token();

        std::string to_string() const;

        static std::string get_typename(int type);

        static const int IDENTIFIER = 146;
        static const int NUMBER = 147;
        static const int STRING = 148;
        static const int CHAR = 149;
        static const int ANNOTATION = 150;
    };

    inline void to_json(json &j, const Token &p) {
        j = json{{"code",     p.type},
                 {"codeName", Token::get_typename(p.type)},
                 {"row",      p.row},
                 {"column",   p.column},
                 {"value",    p.name}};
    }
}

#endif //C_TOKEN_H
