#ifndef NFA_GRAPH_H
#define NFA_GRAPH_H


#include <QString>
#include <QChar>

class nfaNode;//前置声明

class nfaEdge
{
public:
    /* ------------------nfa边节点属性--------------- */
    //特殊的符号“eps”，用于表示epsilon边
    static const QString EPS;

    QString info="";// 边的信息，表示接受的字符类型
    nfaNode *from=nullptr;//起始节点指针
    nfaNode *to=nullptr;// 目标节点指针
    nfaEdge *next=nullptr;// 起始节点的下一条边指针，一个节点可以有多个边，这里采用链表结构存储
    /* ------------------------------------------- */

    nfaEdge(const QString& _info, nfaNode *_from, nfaNode *_to, nfaEdge *_next=nullptr):
        info(_info),from(_from),to(_to),next(_next){};

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

};


class nfaNode
{
public:
    /* ---------nfa节点信息-------- */
    int num;// 节点编号，nfa图中唯一标识一个节点
    nfaEdge *edges=nullptr;// 以该节点为起始的边指针
    /* -------------------------- */

    nfaNode(int _num = 0):num(_num){};
};

#endif // NFA_GRAPH_H
