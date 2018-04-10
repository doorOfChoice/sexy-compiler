//
// Created by dawndevil on 2018/4/7.
//

#include "error_info_exception.h"
#include <sstream>

ErrorInfoException::ErrorInfoException(int row, int column_begin, int column, const std::string &s) : row(row),
                                                                                                      column_begin(
                                                                                                              column_begin),
                                                                                                      column(column) {
    std::ostringstream ost;
    ost << "row: " << row << ", column_begin: " << column_begin << ", column: " << column << ", message: " << s
        << std::endl;
    summary = ost.str();
}

ErrorInfoException::ErrorInfoException() = default;

const char *ErrorInfoException::what() const noexcept {
    return summary.c_str();
}


