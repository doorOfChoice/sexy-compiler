//
// Created by dawndevil on 18-4-14.
//

#ifndef RUN_SYMBOL_H
#define RUN_SYMBOL_H

#include <string>
#include "../plugins/json.hpp"
using json = nlohmann::json;
namespace symbol {
    class Symbol {
    public:
        int code = -1;

        std::string name;

        Symbol() = default;

        Symbol(std::string name, int code) : code(code), name(std::move(name)) {}

        bool operator<(const Symbol &s) const {
            return !(code == s.code && name == s.name);
        }

        bool operator==(const Symbol &s) const {
            return name == s.name;
        }
    };

    inline void to_json(json &j, const Symbol &p) {
        j = json{{"code",     p.code},
                 {"codeName", token::Token::get_typename(p.code)},
                 {"value",    p.name}};
    }
}


#endif //RUN_SYMBOL_H
