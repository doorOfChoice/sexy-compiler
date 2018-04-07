//
// Created by dawndevil on 2018/4/7.
//

#ifndef C_ERRORINFO_H
#define C_ERRORINFO_H


#include <string>
#include <exception>

class ErrorInfoException : public std::exception{
public:
    ErrorInfoException(int row, int column, const char *);
    ErrorInfoException();
    const char * what () const noexcept override;
private:
    char summary[200];
};


#endif //C_ERRORINFO_H
