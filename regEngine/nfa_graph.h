#ifndef NFA_GRAPH_H
#define NFA_GRAPH_H


#include <QString>
#include <QChar>
#include <QMap>
#include <QSet>
#include <QPointer>

class nfaEdge;//前置声明

enum nfaNodeType{start,accept,normal};

class nfaNode
{
public:
    /* ---------nfa节点信息-------- */
    int num;// 节点编号，nfa图中唯一标识一个节点
    nfaNodeType type;
    QSet<QSharedPointer<nfaEdge>> epsEdges;// 以该节点为起始的所有epsilon边
    QMap<QString,QSharedPointer<nfaEdge>> edges;// 以该节点为起始的所有非epsilon边
    /* -------------------------- */

    nfaNode(int _num = 0,nfaNodeType _type=normal):num(_num),type(_type){};

    bool isAccept()
    {
        return type == accept;
    }

    bool isStart()
    {
        return type == start;
    }
};

class nfaEdge
{
public:
    /* ------------------nfa边节点属性--------------- */
    //特殊的符号“eps”，用于表示epsilon边
    static const QString EPS;

    QString info="";// 边的信息，表示接受的字符类型
    QWeakPointer<nfaNode> from;//起始节点指针
    QWeakPointer<nfaNode> to;// 目标节点指针
    /* ------------------------------------------- */

    nfaEdge(const QString& _info, QWeakPointer<nfaNode> _from, QWeakPointer<nfaNode> _to):
        info(_info),from(_from),to(_to){};

    /**
     * @brief isEPS 当前边是否是epsilon边
     * @return
     */
    bool isEPS() const
    {
        return info == EPS;
    }

    /**
     * @brief accept 判断当前边是否能接受字符ch，如果边非空，接受字符后会把ch置空(0)
     * @param ch
     * @return
     * @note 这里就是通过info来判断接受的字符类型，有很大的自由度
     */
    bool accept(QChar ch) const
    {
        if(info == EPS)
        {
            return ch.isNull();
        }
        else
        {
            if(info == "alpha")
            {
                return ch.isLetter();
            }
            else if(info == "digit")
            {
                return ch.isDigit();
            }
            else if(info == "space")
            {
                return ch.isSpace();
            }
            else if(info == "word")
            {
                return ch.isLetterOrNumber() || ch == '_';
            }
            else if(info == "newline")
            {
                return ch == '\n';
            }
            else if(info == "tab")
            {
                return ch == '\t';
            }
            else if(info == "return")
            {
                return ch == '\r';
            }
            else
            {
                return ch == info;
            }
        }
    }

    QString toPrintable()
    {
        QString str;
        QSharedPointer<nfaNode> fromNode = from.toStrongRef();
        QSharedPointer<nfaNode> toNode = to.toStrongRef();
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
            if(toNode->isStart())
            {
                to = " START";
            }
            if(toNode->isAccept())
            {
                to = " ACCEPT";
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

#endif // NFA_GRAPH_H
