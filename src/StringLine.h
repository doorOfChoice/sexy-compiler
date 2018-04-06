#ifndef C_STRINGLINE_H
#define C_STRINGLINE_H

#include <string>
#include <vector>
#include <memory>

class StringLine {
public:
    StringLine() = default;

    StringLine(int line, std::string *text) : line(line), text(text) {}

    ~StringLine() { delete this->text; }

    inline int getLine() { return this->line; }

    inline void setLine(int line) { this->line = line; }

    inline std::string *getText() { return this->text; }

    inline void setText(std::string *text) { this->text = text; }

    /**
     * 转换字符串为StringLine对象，便于之后换取行数和列数
     * @return
     */
    static std::vector<std::shared_ptr<StringLine>> convertString(std::string *);

private:
    //行数
    int line;
    //该行文本
    std::string *text;
};


#endif
