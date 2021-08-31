//
// Created by cloudicen on 2021/8/28.
//

#ifndef MY_COMPILER_REGNODE_H
#define MY_COMPILER_REGNODE_H

#include <string>
#include <memory>
#include <utility>
//#include <format>
#include "../base/baseNode.h"

namespace regEngine {
    enum regNodeType {
        Element,
        Alternate, // '|'
        Concat, // '.'
        Closure, // '*'
    };

    class RegNode : public baseNode {
    public:
        /* -------------节点属性------------- */
        regNodeType type;//类型

        /*
         *节点信息，这里采用了字符串存储，信息更加丰富，例如转义匹配所有数字，没有必要完全按照正则文法构造 ：1|2|3|4....|0,而是用一个digit节点替代，简化了正则语法树
         *在下一步，正则语法树转nfa的时候，就可以根据这里的信息定义转移函数，即一个digit转移边可以接受所有属于数字的字符
         *在nfa转dfa的时候，再根据这里的信息构造实际的字符表
         */
        std::string info;

        //子节点，正则语法树是一个二叉树，故有两个子节点
        std::unique_ptr<RegNode> leftChildNode;
        std::unique_ptr<RegNode> rightChildNode;
        /* -------------------------------- */
    public:
        explicit RegNode(regNodeType _type = regNodeType::Element, const std::string& _info = "",
                         std::unique_ptr<RegNode> _leftChildNode = nullptr,
                         std::unique_ptr<RegNode> _rightChildNode = nullptr)
                : baseNode(),
                  type(_type), info(_info),
                  leftChildNode(std::move(_leftChildNode)), rightChildNode(std::move(_rightChildNode)) {};

        std::string toPrintable() override {
            using std::string;
            //string str = "[info= \'{}\'](type = {{}})";
            string str = "[info= \'";
            if (info.empty()) {
                //str = std::format(str, "empty_node");
                str.append("empty_node");
            } else {
                //str = std::format(str, info);
                str.append(info);
            }
            str.append("\'](type = ");
            switch (type) {
                case Element:
                    //str = std::format(str, "element");
                    str.append("element");
                    break;
                case Alternate:
                    //str = std::format(str, "alternate");
                    str.append("alternate");
                    break;
                case Concat:
                    //str = std::format(str, "concat");
                    str.append("concat");
                    break;
                case Closure:
                    //str = std::format(str, "closure");
                    str.append("closure");
                    break;
                default:
                    //str = std::format(str, "undefined");
                    str.append("undefined");
            }
            str.append(")");
            return str;
        }
    };
}

#endif //MY_COMPILER_REGNODE_H
