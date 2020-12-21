#ifndef NFA_H
#define NFA_H

#include "nfa_graph.h"
#include "reg_tree.h"
#include <QMultiMap>
#include <iostream>
#include <iomanip>

class nfa
{
private:
    /* ---------需要转换的正则语法树-------- */
    regTree tree;//语法树根节点
    /* --------------------------------- */


    /* -----------nfa图相关属性------------ */
    int start=0;// 开始状态节点编号
    int accept=0;// 接受状态节点编号
    QMap<int,QSharedPointer<nfaNode>> nodes;// 所有节点存储为一个map，因为每个顶点都有一个唯一的标识符num, 用num来索引顶点指针
    QMultiMap<QString,QSharedPointer<nfaEdge>> edges; // 把所有非EPS边存储为一个multimap, 根据边上的字符来索引边的指针
    bool valid=false;
    /* --------------------------------- */


    /* -----------nfa图构造时变量------------ */
    int nodeNumber=0;// 节点数量
    /* ----------------------------------- */


    /* -----------nfa图构造方法------------ */
    void Thompson(const QWeakPointer<const regNode> &subTree);// Thompson算法
    void addEdge(int from, int to, QString info);// 添加一条边
    QSharedPointer<nfaNode> getNode(int num);// 获取一个节点，若不存在则创建
    /* ----------------------------------- */

public:
    nfa()=default;
    nfa(regTree &_tree):tree(std::move(_tree)){};
    nfa(const nfa&)=delete;
    nfa(nfa&& other)
    {
        //移动图结构
        nodes = std::move(other.nodes);
        edges = std::move(other.edges);
        tree = std::move(other.tree);

        //赋值&重置图参数
        start = other.start;
        accept = other.accept;
        valid = other.valid;
        nodeNumber = other.nodeNumber;

        other.start=0;
        other.accept=0;
        other.valid=0;
        other.nodeNumber=0;
    }

    nfa operator=(nfa&& other)
    {
        return nfa(std::move(other));
    }

    QMap<int,QSharedPointer<const nfaNode>> getAllNodes()
    {
        QMap<int,QSharedPointer<const nfaNode>> constRef;
        for(auto item=nodes.begin();item!=nodes.end();item++)
        {
            constRef[item.key()] = item.value();
        }
        return constRef;
    }

    QMultiMap<QString,QSharedPointer<const nfaEdge>> getAllEdges()
    {
        QMultiMap<QString,QSharedPointer<const nfaEdge>> constRef;
        for(auto item=edges.begin();item!=edges.end();item++)
        {
            constRef.insert(item.key(),item.value());
        }
        return constRef;
    }

    const QSharedPointer<const nfaNode> findNode(int node)
    {
        return nodes.value(node);
    }

    QList<QSharedPointer<const nfaEdge>> findEdges(const QString& info)
    {
        QList<QSharedPointer<const nfaEdge>> constRef;
        auto edgeSet = edges.values(info);
        for(auto item=edgeSet.begin();item!=edgeSet.end();item++)
        {
            constRef.push_back(*item);
        }
        return constRef;
    };

    int getStartNode()
    {
        return start;
    };

    int getAcceptNode()
    {
        return accept;
    };

    void parseNFA();

    void testNFA(QString str);

    void print();

    bool isValid()
    {
        return valid;
    }

    QSet<int> getEPSclosure(QSet<int> start);// 求初态epsilon闭包
    QSet<int> getSMove(QSet<int> current,QChar ch);// 求epsilon闭包

};

#endif // NFA_H
