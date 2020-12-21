#ifndef REG_NODE_H
#define REG_NODE_H

#include <QString>
#include <QSharedPointer>
#include <QWeakPointer>
/**
 * @brief The regNodeType enum
 * 这里对应thompson算法中的几个规则
 * 基本规则：element ->子表达式
 * 归纳规则：alternate -> 并规则
 *          concat -> 交规则
 *          closure -> 闭包规则
 */
enum regNodeType
{
    Element,
    Alternate, // '|'
    Concat, // '.'
    Closure, // '*'
};

struct regNode
{
    /* -------------节点属性------------- */
    regNodeType type;//类型

    /*
     *节点信息，这里采用了字符串存储，信息更加丰富，例如转义匹配所有数字，没有必要完全按照正则文法构造 ：1|2|3|4....|0,而是用一个digit节点替代，简化了正则语法树
     *在下一步，正则语法树转nfa的时候，就可以根据这里的信息定义转移函数，即一个digit转移边可以接受所有属于数字的字符
     *在nfa转dfa的时候，再根据这里的信息构造实际的字符表
     */
    QString info;

    QWeakPointer<regNode> children[2];//子节点，正则语法树是一个二叉树，故有两个子节点
    /* -------------------------------- */

    regNode(regNodeType _type,QString _info,QSharedPointer<regNode> leftChild=QSharedPointer<regNode>(),QSharedPointer<regNode> rightChild=QSharedPointer<regNode>()):
        type(_type),info(_info),children{leftChild,rightChild}{};

    /**
     * @brief isLeaf 当前节点是否是叶子节点
     * @return
     */
    bool isLeaf() const
    {
        return children[0].isNull() && children[1].isNull();
    }

    /**
     * @brief toPrintable
     * @return
     */
    QString toPrintable() const
    {
        QString str="[token = \'%1\'](type = %2)";
        if(info.isEmpty())
        {
            str = str.arg("\x0");
        }
        else
        {
            str = str.arg(info);
        }
        switch (type)
        {
        case Element:
            str=str.arg("element");
            break;
        case Alternate:
            str=str.arg("alternate");
            break;
        case Concat:
            str=str.arg("concat");
            break;
        case Closure:
            str=str.arg("closure");
            break;
        default:
            str=str.arg("undefined");
        }
        return str;
    }
};

#endif // REG_NODE_H
