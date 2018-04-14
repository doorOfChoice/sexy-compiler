//
// Created by dawndevil on 2018/4/6.
//
#include <iostream>
#include <vector>
#include <memory>
#include "string_line.h"
#include "error_info_exception.h"
#include "string_util.h"
/**
 * 有限状态机进行去注释，但是保留相应的行数和列数
 *
 * 本状态机主要分为6个状态
 * 状态0: 扫描到第一个/或者普通字符，可以跳转 状态1 和 状态6
 * 状态1: 扫描到第二个/, 可以跳转 状态2 和 状态四
 * 状态6: 扫描到普通字符, 可以跳转到 状态7 和 状态1 和 状态3
 * 状态2: 扫描到非换行符, 可以跳转到 状态2 和 状态3
 * 状态4: 扫描到非*字符, 可以跳转到 状态4 和 状态5
 * 状态5: 扫描到最后一个/，可以跳转 状态4 和 状态0
 * 状态3: 扫描的换行符, 可以跳转 状态0 和 状态4
 * @param s 需要进行分割的文本
 * @return StringLine集合 + 错误信息列表
 */
std::pair<std::vector<std::shared_ptr<StringLine>>, std::vector<errorinfo::ErrorInfoException>>
StringLine::convert_string(const std::string *s) {
    std::vector<std::shared_ptr<StringLine>> sls;
    std::vector<errorinfo::ErrorInfoException> errors;
    int lineNumber = 1;
    auto it = s->begin();
    //StringLine
    auto sl = std::make_shared<StringLine>(lineNumber, std::string());
    int state = 0;
    //判断当前扫描的字符串是否在注释中
    bool isAnnotation = false;
    //判断注释是否在字符串中
    bool annotationInString = false;
    while (it != s->end()) {
        switch (state) {
            //初始读取状态，判断是注释还是普通字符
            case 0: {
                //注释如果在字符里面，就不进行跳转，当作普通字符处理
                if (*it == '/' && !annotationInString)state = 1;
                else {
                    state = 6;
                    --it;
                }
                break;
            }
                //判断第二个/，或者第一个*
            case 1: {
                if (*it == '/')state = 2;
                else if (*it == '*')state = 4;
                else {
                    state = 0;
                    --it;
                }
                break;
            }
                // 判断是//注释，还是换行停止
            case 2: {
                if (*it == '\n') {
                    state = 3;
                    --it;
                } else {
                    sl->get_text().push_back(' ');
                    state = 2;
                }
                break;
            }
                //处理已经读取了一行的情况
            case 3: {
                if (!sutil::trim(sl->get_text()).empty()) {
                    sl->get_text().push_back('\n');
                    sls.push_back(sl);
                }
                sl = std::make_shared<StringLine>(lineNumber, std::string());
                ++lineNumber;
                state = isAnnotation ? 4 : 0;
                break;
            }
                // 处理/*注释
            case 4: {
                if (*it == '*')state = 5;
                else {
                    isAnnotation = true;
                    if (*it == '\n') {
                        state = 3;
                        --it;
                    } else {
                        sl->get_text().push_back(' ');
                        state = 4;
                    }
                }
                break;
            }
                // 判断*后面是否是/
            case 5: {
                if (*it == '/') {
                    sl->get_text().append("    ");
                    isAnnotation = false;
                    state = 0;
                } else {
                    state = 4;
                    --it;
                }
                break;
            }
                //读取普通字符
            case 6: {
                if (*it == '"') annotationInString = !annotationInString;
                if (*it == '/' && !annotationInString) {
                    state = 0;
                    --it;
                } else if (*it != '\n') {
                    sl->set_line(lineNumber);
                    sl->get_text().push_back(*it);
                } else {
                    state = 3;
                    --it;
                }
            }
            default: {
            }
        }
        ++it;
    }
    if(!sl->get_text().empty()) {
        sl->get_text().push_back('\n');
        sls.push_back(sl);
    }
    if (isAnnotation) {
        errors.emplace_back(lineNumber, 0, 0,  "Brother, where is your annotation tail?");
    }
    return std::make_pair(sls, errors);
}