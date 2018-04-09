//
// Created by dawndevil on 2018/4/7.
//

#ifndef C_ERRORINFO_H
#define C_ERRORINFO_H


#include <string>
#include <exception>

class error_info_exception : public std::exception{
public:
    error_info_exception(int row, int column, const std::string &);
    error_info_exception();
    const char * what () const noexcept override;
private:
    std::string summary;
};


#endif //C_ERRORINFO_H
