#ifndef REG_TREE_H
#define REG_TREE_H

#include "reg_node.h"
#include <QPair>
#include <QString>
#include <QVector>
#include <QDebug>

/**
 * @brief The reg_tree class
 * @note
 * 目前的实现中，只支持：【选择、连接、闭包】运算，由于每个字符都是一个词法单元，故直接构造一个语法制导翻译器，其输出是一个正则语法树
 * 终结符号列表：
 * {
 *     '|' -> 选择
 *     '.' -> 连接
 *     '*' -> 闭包
 *     '(' -> 左括号
 *     ')' -> 右括号
 *     '\' -> 反斜杠，用于转义控制字符
 *     char -> 任意除保留字以外的可打印字符
 * }
 * 非终结符列表：
 * {
 *     regExpr -> 正则表达式（开始符号）
 *     EscapeChar -> 转义字符
 * }
 * 构造产生式集合（优先级从上到下递增）：
 * {
 *     regExpr -> regExpr|regExpr
 *             -> regExpr.regExpr
 *             -> regExpr*
 *             -> (regExpr)
 *             -> \EscapeChar
 *             -> char
 *     EscapeChar -> |
 *                -> .
 *                -> \
 *                -> *
 *                -> (
 *                -> )
 *                -> \r
 *                -> \t
 *                -> \n
 *                -> s (空白字符）
 *                -> w (digit|alpha|_)
 *                -> d (digit)
 *                -> a (alpha)
 * }
 * ->消除左递归，提取左因子，以便自顶向下分析
 * {
 *     regExpr -> |regExpr subExpr
 *             -> .regExpr subExpr
 *             -> * subExpr
 *             -> (regExpr) subExpr
 *             -> \EscapeChar subExpr
 *             -> char subExpr
 *
 *     subExpr -> |regExpr regExpr
 *             -> .regExpr regExpr
 *             -> * regExpr
 *             -> 空
 * }
 * ->最终的非终结符列表
 * {
 *     regExpr -> 正则表达式（开始符号）
 *     subExpr -> 子表达式
 *     EscapeChar -> 转义字符
 * }
 */

class regTree
{
private:
    /* -------------------------需要转换的表达式----------------------------- */
    QString regExpr;// exp用来存储正则表达式
    /* ------------------------------------------------------------------- */


    /* ---------------------------关键字定义------------------------------- */
    QSet<QChar> keptChar = {'|','.','\\','*','(',')'};// 保留符号


    //转义符号
    QMap<QChar,QString> escapeChar = {
        {'w',"word"}, // 转义匹配 数字+字母+下划线
        {'a',"alpha"},// 转义匹配 字母
        {'d',"digit"},// 转义匹配 数字
        {'s',"space"},// 转义匹配 空白字符
        {'n',"newline"},// 转义匹配 换行符
        {'t',"tab"},// 转义匹配 制表符
        {'r',"return"},// 转义匹配 回车

        {'\\',"\\"},// 转义匹配保留符号
        {'|',"|"},
        {'.',"."},
        {'*',"*"},
        {'(',"("},
        {')',")"}
    };
    /* ------------------------------------------------------------------- */


    /* -----------------------------表达式语法树---------------------------- */
    regNode* root;// 指向正则表达式树的根节点
    QVector<regNode*> nodes;// 将正则树上所有节点的指针保存到表中，方便遍历释放
    bool analyzed=false;
    /* ------------------------------------------------------------------- */


    /* ---------------------- 表达式树构造时相关变量 --------------------------*/
    int pos=-1;// 指向exp的位置指针
    QChar currentChar=0;// 保存当前的字符
    QVector<QChar> signStack;// 成对出现符号的匹配栈
    /* --------------------------------------------------------------------- */


    /* --------------------------表达式树构造方法--------------------------- */
    regNode* parse_regExpr();//递归下降子程序 -> 处理非终结符regExpr
    regNode* parse_subExpr();//递归下降子程序 -> 处理非终结符subExpr
    regNode* parse_EscapeChar();//递归下降子程序 -> 处理非终结符escapeChar

    void getNextChar();// 从表达式串中获取下一个字符
    void putCharback();// 将当前字符退回输入流
    bool match(QChar ch);// 匹配当前字符

    bool matchSign(QChar ch);// 匹配成对出现的符号
    /* --------------------------------------------------------------------- */

public:
    regTree(const QString& _regExpr):regExpr(_regExpr){};

    /**
     * @brief getRootNode 获得根节点指针
     * @return
     */
    const regNode* getRootNode() const
    {
        return root;
    }

    /**
     * @brief moveTree 将分析后的树结构所有权移出
     * @return <树的根节点,所有节点指针的列表>
     */
    QPair<regNode*,QVector<regNode*>> moveTree()
    {
        auto tree = qMakePair(root,nodes);
        root = nullptr;
        nodes.clear();
        analyzed=false;
        return tree;
    }

    /**
     * @brief analyze 生成分析树
     * @return 无文法错误返回true
     */
    bool analyze();

    /**
     * @brief isAnalyzed 当前对象是否分析并生成了语法树
     * @return
     */
    bool isAnalyzed()
    {
        return analyzed;
    }
};

#endif // REG_TREE_H
