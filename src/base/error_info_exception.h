//
// Created by dawndevil on 2018/4/7.
//

#ifndef C_ERRORINFO_H
#define C_ERRORINFO_H


#include <string>
#include <exception>
#include "../plugins/json.hpp"

using json = nlohmann::json;

namespace errorinfo {
    class ErrorInfoException : public std::exception {
    public:
        ErrorInfoException(int row, int column_begin, int column, const std::string &);

        ErrorInfoException();

        const char *what() const noexcept override;

        int get_row() const { return row; }

        int get_column_begin() const { return column_begin; }

        int get_column() const { return column; }

    private:
        int row;
        int column_begin;
        int column;
        std::string summary;
    };

    inline void to_json(json &j, const ErrorInfoException &p) {
        j = json{{"row",          p.get_row()},
                 {"column_begin", p.get_column_begin()},
                 {"column_end",   p.get_column()},
                 {"summary",      p.what()}
        };
    }
}

#endif //C_ERRORINFO_H
