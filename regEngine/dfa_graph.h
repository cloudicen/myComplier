#ifndef DFA_GRAPH_H
#define DFA_GRAPH_H

#include <QString>
#include <QChar>
#include <QSet>

class dfaNode;

class dfaEdge
{
public:
    /* --------------dfa边属性---------- */
    QChar info;// dfa边的接受字符
    dfaNode *from;// 起始节点指针
    dfaNode *to;// 目标节点指针
    dfaEdge *next;// 起始节点的下一个dfa边
    /* -------------------------------- */

    dfaEdge(const QChar& _info, dfaNode *_from, dfaNode *_to, dfaEdge *_next):
        info(_info),from(_from),to(_to),next(_next){};

    /**
     * @brief accept 返回当前边是否接受字符ch
     * @param ch
     * @return
     */
    bool accept(QChar ch) const
    {
        return ch == info;
    }

};


class dfaNode
{
public:
    /* -----------dfa节点属性---------- */
    int num;//节点编号，在一个dfa图中唯一标识节点
    QSet<int> closure;// 节点代表的nfa状态集
    bool accept=false;// 标识接受节点
    bool dead=false;
    dfaEdge *edges=nullptr;// 节点的边
    /* ------------------------------ */
    dfaNode(int _num = 0,const QSet<int>& _closure = QSet<int>()):num(_num),closure(_closure){};

    /**
     * @brief isAccept 返回节点是否是接受节点
     * @return
     */
    bool isAccept() const
    {
        return accept;
    }

    bool isDead() const
    {
        return dead;
    }
};

#endif // DFA_GRAPH_H
