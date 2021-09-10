//
// Created by cloudicen on 2021/8/28.
//

#ifndef MY_COMPILER_PRINTABLE_H
#define MY_COMPILER_PRINTABLE_H

#include <string>

/**
 * 所有节点类的基类
 */
class Printable {
public:
    virtual std::string toPrintable() const = 0;
    virtual ~Printable() = default;
};


#endif // MY_COMPILER_PRINTABLE_H
