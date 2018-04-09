//
// Created by dawndevil on 2018/4/7.
//

#include "error_info_exception.h"
#include <sstream>

error_info_exception::error_info_exception(int row, int column, const std::string &s) {
    std::ostringstream ost;
    ost << "row: " << row << ", column: " << column << ", message: " << s << std::endl;
    summary = ost.str();
}

error_info_exception::error_info_exception() = default;

const char *error_info_exception::what() const noexcept {
    return summary.c_str();
}


