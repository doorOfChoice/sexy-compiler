//
// Created by dawndevil on 2018/4/7.
//

#include "ErrorInfoException.h"

ErrorInfoException::ErrorInfoException(int row, int column, const char *s) {
    sprintf(summary, "row: %d, column: %d, message: %s", row, column, s);
}

ErrorInfoException::ErrorInfoException() = default;

const char *ErrorInfoException::what() const noexcept {
    return summary;
}


