#ifndef C_STRINGLINE_H
#define C_STRINGLINE_H

#include <string>
#include <utility> #include <vector>
#include <memory>

class StringLine {
public:
    StringLine() = default;

    StringLine(int line, std::string text) : line(line), text(std::move(text)) {}

    ~StringLine() = default;

    int getLine() { return line; }

    void setLine(int line) { this->line = line; }

    std::string &getText() { return text; }

    void setText(std::string text) { this->text = std::move(text); }

    /**
     * 转换字符串为StringLine对象，便于之后换取行数和列数
     * @return
     */
    static std::vector<std::shared_ptr<StringLine>> convertString(std::string *);

private:
    //行数
    int line;
    //该行文本
    std::string text;
};


#endif
