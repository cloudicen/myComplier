//
// Created by cloudicen on 2021/8/28.
//

#ifndef MY_COMPILER_BASENODE_H
#define MY_COMPILER_BASENODE_H

#include <string>

/**
 * 所有节点类的基类
 */
class baseNode {
public:
    virtual std::string toPrintable() = 0;
    virtual ~baseNode() = default;
};


#endif //MY_COMPILER_BASENODE_H
