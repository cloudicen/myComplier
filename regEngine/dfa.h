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
    QMap<int,QSharedPointer<dfaNode>> nodes;// 把所有的顶点存储为一个map, 因为每个顶点都有一个唯一的标识符num, 用num来索引顶点指针
    QMultiMap<QChar,QSharedPointer<dfaEdge>> edges;// 把所有的边存储为一个multimap, 根据边上的字符来索引边的指针
    QSet<QChar> alphabet;// 字符表
    bool valid=false;
    /* --------------------------------*/


    /* ---------- dfa图构造时相关变量 ---------*/
    int nodeNumber=0;// 节点数
    bool needNewNode=false;// 构造的过程中添加新节点的标志位
    /* -------------------------------------*/


    /* -------------- dfa构造方法 -------------- */
    void subsetConstruct();// 子集构造法
    void addEdge(QSet<int> &from, QSet<int> &to, QChar ch);// 添加一条边
    QSharedPointer<dfaNode> getNode(QSet<int> closure);// 获取一个节点，若不存在则创建
    void parseAlphabet();// 根据nfa图获取字符表
    /* ----------------------------------------*/

public:
    dfa()=default;
    dfa(nfa&ins):
        NFAgraph(std::move(ins)),
        nfaStart(NFAgraph.getStartNode()),
        nfaAccept(NFAgraph.getAcceptNode()){};

    dfa(const dfa&)=delete;
    dfa(dfa&& other)
    {
        NFAgraph = std::move(other.NFAgraph);
        nodes = std::move(other.nodes);
        edges = std::move(other.edges);
        alphabet = std::move(other.alphabet);

        valid = other.valid;
        nfaStart = other.nfaStart;
        nfaAccept = other.nfaAccept;
        start = other.start;

        //原有的对象置空；
        other.valid=0;
        other.nfaStart=0;
        other.nfaAccept=0;
        other.start=0;
        other.needNewNode=false;
        other.nodeNumber=0;
    }

    dfa operator=(dfa&& other)
    {
        return dfa(std::move(other));
    }

    QMap<int,QSharedPointer<const dfaNode>> getAllNodes()
    {
        QMap<int,QSharedPointer<const dfaNode>> constRef;
        for(auto item=nodes.begin();item!=nodes.end();item++)
        {
            constRef[item.key()] = item.value();
        }
        return constRef;
    }

    QMultiMap<QString,QSharedPointer<const dfaEdge>> getAllEdges()
    {
        QMultiMap<QString,QSharedPointer<const dfaEdge>> constRef;
        for(auto item=edges.begin();item!=edges.end();item++)
        {
            constRef.insert(item.key(),item.value());
        }
        return constRef;
    }

    const QSharedPointer<const dfaNode> findNode(int node)
    {
        return nodes.value(node);
    }

    QList<QSharedPointer<const dfaEdge>> findEdges(const QChar& info)
    {
        QList<QSharedPointer<const dfaEdge>> constRef;
        auto edgeSet = edges.values(info);
        for(auto item=edgeSet.begin();item!=edgeSet.end();item++)
        {
            constRef.push_back(*item);
        }
        return constRef;
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

    void parseDFA();

    void testDFA(QString str);

    void toPrintable();
};

#endif // DFA_H
