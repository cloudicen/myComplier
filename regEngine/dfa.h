#ifndef DFA_H
#define DFA_H

#include "dfa_graph.h"
#include "nfa.h"

class dfa
{
private:
    /* ---------- 要转换的nfa图 --------- */
    nfa NFAgraph;
    int nfaStart;
    int nfaAccept;
    /* --------------------------------*/


    /* ---------- dfa图相关属性 ---------*/
    int start=0;// 开始状态节点，根据dfa的构造方式，开始节点始终是0
    QMap<int,dfaNode *> nodes;// 把所有的顶点存储为一个map, 因为每个顶点都有一个唯一的标识符num, 用num来索引顶点指针
    QMultiMap<QChar, dfaEdge*> edges;// 把所有的边存储为一个multimap, 根据边上的字符来索引边的指针
    QSet<QChar> alphabet;// 字符表
    bool valid=false;
    /* --------------------------------*/


    /* ---------- dfa图构造时相关变量 ---------*/
    int nodeNumber=0;// 节点数
    bool needNewNode=false;// 构造的过程中添加新节点的标志位
    /* -------------------------------------*/


    /* -------------- dfa构造方法 -------------- */
    void subsetConstruct();// 子集构造法
    void addEdge(QSet<int> from, QSet<int> to, QChar ch);// 添加一条边
    dfaNode* getNode(QSet<int> closure);// 获取一个节点，若不存在则创建
    void parseAlphabet();// 根据nfa图获取字符表
    /* ----------------------------------------*/

public:
    dfa(nfa &ins):
        NFAgraph(std::move(ins)),
        nfaStart(NFAgraph.getStartNode()),
        nfaAccept(NFAgraph.getAcceptNode()){};

    void parseDFA();

    QMap<int,dfaNode *> getAllNodes()
    {
        return nodes;
    };

    QMultiMap<QChar, dfaEdge*> getAllEdges()
    {
        return edges;
    };

    QSet<QChar> getAlphabet()
    {
        return alphabet;
    }

    int getStartNode()
    {
        return start;
    }

    bool isValid()
    {
        return valid;
    }

    void testDFA(QString str);

    void toPrintable();
};

#endif // DFA_H
