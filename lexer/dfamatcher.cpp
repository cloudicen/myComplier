#include "dfamatcher.h"

int dfaMatcher::match(const QString &str)
{
    auto node = startNode.toStrongRef();
    if(!node)
    {
        return -1;
    }
    bool hasEdge = false;
    int lastMatchPos=-1;
    int pos=0;
    foreach(QChar ch,str)
    {
        auto edge=node->edges.value(ch);
        hasEdge = false;
        if(!edge.isNull())
        {
            node = edge->to;
            hasEdge = true;
            if(node->isAccept())
            {
                lastMatchPos=pos;
            }
        }
        if(node->isDead() || !hasEdge)//对当前输入无状态转移边，直接结束循环
        {
            break;
        }
        pos++;
    }
    return lastMatchPos;
}
