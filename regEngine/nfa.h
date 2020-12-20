#ifndef NFA_H
#define NFA_H

#include "nfa_graph.h"
#include "reg_tree.h"
#include <QMultiMap>

class nfa
{
private:
    /* ---------需要转换的正则语法树-------- */
    const regNode* regTreeRoot;//语法树根节点
    /* --------------------------------- */


    /* -----------nfa图相关属性------------ */
    int start=0;// 开始状态节点编号
    int accept=0;// 接受状态节点编号
    QMap<int,nfaNode *> nodes;// 所有节点存储为一个map，因为每个顶点都有一个唯一的标识符num, 用num来索引顶点指针
    QMultiMap<QString,nfaEdge*> edges; // 把所有非EPS边存储为一个multimap, 根据边上的字符来索引边的指针
    bool valid=false;
    /* --------------------------------- */


    /* -----------nfa图构造时变量------------ */
    int nodeNumber=0;// 节点数量
    /* ----------------------------------- */


    /* -----------nfa图构造方法------------ */
    void Thompson(const regNode *subTree);// Thompson算法
    void addEdge(int from, int to, QString info);// 添加一条边
    nfaNode* getNode(int num);// 获取一个节点，若不存在则创建
    /* ----------------------------------- */

public:
    nfa(regTree &tree):regTreeRoot(tree.getRootNode()){};

    /**
     * @brief getAllNodes 返回节点map的const引用
     * @return
     */
    QMap<int,nfaNode *> getAllNodes()
    {
        return nodes;
    };

    /**
     * @brief getAllEdges 返回边节点的const引用
     * @return
     */
    QMultiMap<QString,nfaEdge*> getAllEdges()
    {
        return edges;
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

    void toPrintable();

    bool isValid()
    {
        return valid;
    }

};

#endif // NFA_H
