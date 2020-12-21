#ifndef DFA_GRAPH_H
#define DFA_GRAPH_H

#include <QString>
#include <QPointer>
#include <QMap>
#include <QChar>
#include <QSet>

class dfaEdge;

class dfaNode
{
public:
    /* -----------dfa节点属性---------- */
    int num;//节点编号，在一个dfa图中唯一标识节点
    QSet<int> closure;// 节点代表的nfa状态集
    bool accept=false;// 标识接受节点
    bool dead=false;
    bool start=false;
    QMap<QChar,QSharedPointer<dfaEdge>> edges;
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

    bool isStart() const
    {
        return start;
    }
};

class dfaEdge
{
public:
    /* --------------dfa边属性---------- */
    QChar info;// dfa边的接受字符
    QWeakPointer<dfaNode> from;// 起始节点指针
    QWeakPointer<dfaNode> to;// 目标节点指针
    /* -------------------------------- */

    dfaEdge(const QChar& _info,QWeakPointer<dfaNode> _from,QWeakPointer<dfaNode> _to):
        info(_info),from(_from),to(_to){};

    /**
     * @brief accept 返回当前边是否接受字符ch
     * @param ch
     * @return
     */
    bool accept(QChar ch) const
    {
        return ch == info;
    }

    QString toPrintable()
    {
        QString str;
        QSharedPointer<dfaNode> fromNode = from.toStrongRef();
        QSharedPointer<dfaNode> toNode = to.toStrongRef();
        if(!fromNode.isNull() && !toNode.isNull())
        {
            QString from="         ",to="";
            if(fromNode->isStart())
            {
                from = "  START  ";
            }
            if(fromNode->isAccept())
            {
                from = " ACCEPT  ";
            }
            if(fromNode->isDead())
            {
                from = " DEAD    ";
            }
            if(toNode->isStart())
            {
                to = " START";
            }
            if(toNode->isAccept())
            {
                to = " ACCEPT";
            }
            if(toNode->isDead())
            {
                to = " DEAD    ";
            }
            str.append(from);
            str.append(QString().setNum(fromNode->num));
            str.append(" -----( ");
            str.append(info);
            str.append(" )----> ");
            str.append(QString().setNum(toNode->num));
            str.append(to);
        }
        return str;
    }

};

#endif // DFA_GRAPH_H
