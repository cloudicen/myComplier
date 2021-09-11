//
// Created by cloudicen on 2021/8/28.
//

#ifndef MY_COMPILER_PRINTABLE_H
#define MY_COMPILER_PRINTABLE_H

#include <string>

/**
 * 所有可打类型基类
 */
class Printable {
public:
    Printable() = default;
    virtual ~Printable() = default;
    virtual std::string toPrintable() const = 0;
};


#endif // MY_COMPILER_PRINTABLE_H
