/**
 * 字符串行
 * 可以轻松的获取某一个字符串在源文本中的列数和行数
 */
#ifndef C_STRINGLINE_H
#define C_STRINGLINE_H

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include "error_info_exception.h"

class StringLine {
public:
    StringLine() = default;

    StringLine(int line, std::string text) : line(line), text(std::move(text)) {}

    ~StringLine() = default;

    int get_line() { return line; }

    void set_line(int line) { this->line = line; }

    std::string &get_text() { return text; }

    void set_text(std::string text) { this->text = std::move(text); }

    /**
     * 转换字符串为StringLine对象，便于之后换取行数和列数
     * @return
     */
    static std::pair<std::vector<std::shared_ptr<StringLine>>, std::vector<ErrorInfoException>> convert_string(const std::string *);

private:
    //行数
    int line;
    //该行文本
    std::string text;
};


#endif
