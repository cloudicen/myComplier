//
// Created by cloudicen on 2021/8/29.
//

#ifndef MY_COMPILER_REGENGINEDEF_H
#define MY_COMPILER_REGENGINEDEF_H

#include <string>
#include <functional>
#include <unordered_map>
#include <unordered_set>

namespace regEngine {

/* ---------------------------关键字定义------------------------------- */
static std::unordered_set<char> keptCharSet{'|', '*', '(', ')',
                                            '\\'}; // 保留符号
static std::unordered_map<char, std::string> escapeCharDef{
    {'w', "word"},    // 转义匹配 数字+字母+下划线
    {'a', "alpha"},   // 转义匹配 字母
    {'d', "digit"},   // 转义匹配 数字
    {'s', "space"},   // 转义匹配 空白字符
    {'n', "newline"}, // 转义匹配 换行符
    {'t', "tab"},     // 转义匹配 制表符
    {'r', "return"},  // 转义匹配 回车
    {'e', "empty"}, // 转义匹配空串，辅助实现‘？’算符的零次或一次匹配

    {'\\', "\\"}, // 转义匹配保留符号
    {'|', "|"},       {'.', "."}, {'*', "*"},
    {'(', "("},       {')', ")"}};                            //转义符号
static std::unordered_map<char, char> signInPair{{'(', ')'}}; // 成对出现的符号

static std::unordered_map<std::string,std::string> info2TokenDef {
        {"word","abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_"},
        {"alpha","abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"},
        {"digit","1234567890"},
        {"space"," \n\t\r"},
        {"newline","\n"},
        {"tab","\t"},
        {"return","\r"},
};

static std::unordered_map<std::string,std::function<bool(char)>> matchRules{
        {"word",[](char ch){return false;}}
};
/* ------------------------------------------------------------------- */
} // namespace regEngine

#endif // MY_COMPILER_REGENGINEDEF_H
