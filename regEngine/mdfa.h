#ifndef MDFA_H
#define MDFA_H

#include "dfa.h"
#include <QPair>

class mdfa
{
private:
    /* ---------- 要最小化的dfa图 --------- */
    QMap<int,dfaNode *> dfaNodes;
    QMultiMap<QChar, dfaEdge*> dfaEdges;
    QSet<QChar> alphabet;
    /* ---------------------------------*/


    /* ---------- 最小化dfa图相关属性 ---------*/
    int start = 0;//起始节点编号
    QMap<int,dfaNode *> nodes;// 把所有的顶点存储为一个map, 因为每个顶点都有一个唯一的标识符num, 用num来索引顶点指针
    QMultiMap<QChar,dfaEdge*> edges;// 把所有的边存储为一个multimap, 根据边上的字符来索引边的指针
    bool valid=false;
    /* ------------------------------------*/


    /* ---------- 最小化dfa图构造时相关变量 ---------*/
    int nodeNumber=0;//顶点数
    QSet<QSet<int>> finalGroups;//在Hopcroft算法中,需要用来判断两次划分有没有变化, P用来保存前一次划分的结果, T用来保存划分后的结果
    /* -----------------------------------------*/


    /* -------------- 最小化dfa构造方法 -------------- */
    QPair<QSet<int>, QSet<int>> split(QSet<int> S);// 子划分算法
    void hopcroft();//Hopcroft算法
    void addEdge(int from, int to, QChar ch);//新建一条边

public:
    mdfa(dfa ins):dfaNodes(ins.getAllNodes()),dfaEdges(ins.getAllEdges()),alphabet(ins.getAlphabet()){};

    bool isValid()
    {
        return valid;
    }

    void parseMDFA();

    void testMDFA(QString str);

    void print();

    bool match(const QString &str);
};

#endif // MDFA_H
