//
// Created by dawndevil on 2018/4/7.
//

#ifndef C_ERRORINFO_H
#define C_ERRORINFO_H


#include <string>
#include <exception>

class ErrorInfoException : public std::exception {
public:
    ErrorInfoException(int row, int column_begin, int column, const std::string &);

    ErrorInfoException();

    const char *what() const noexcept override;

    int get_row() const { return row; }

    int get_column_begin()const { return column_begin; }

    int get_column() const { return column; }

private:
    int row;
    int column_begin;
    int column;
    std::string summary;
};


#endif //C_ERRORINFO_H
