#include "mdfa.h"

QPair<QSet<int>, QSet<int> > mdfa::split(QSet<int> nodeGroup)
{
    if(nodeGroup.size()==1)
    {
        qDebug() << nodeGroup << "can't spilt";
        return qMakePair(nodeGroup,nodeGroup);
    }
    auto source = nodeGroup;
    for (auto c = alphabet.begin(); c != alphabet.end(); ++c)
    {
        QSet<int> s1, s2;
        auto edgeGroup = dfaEdges.values(*c);
        foreach(auto edge,edgeGroup)
        {
            auto from = edge->from->num;
            auto to = edge->to->num;
            foreach(auto node,nodeGroup)
            {
                if(node == from)
                {
                    qDebug() << "CHECK EDGE:" << from << "----" << *c << "----->" << to;
                    //不可区分，保留分组
                    if(nodeGroup.contains(to))
                    {
                        s1.insert(from);
                    }
                    else
                    {
                        s2.insert(from);
                    }
                }
            }
        }
        //存在不可区分的分组，直接返回，否则测试下一组符号
        if (s1 != nodeGroup)
        {
            qDebug() << nodeGroup << "can spilt to:" << s1 << s2;
            return qMakePair(s1,s2);
        }
    }
    qDebug() << nodeGroup << "can't spilt";
    return qMakePair(nodeGroup,nodeGroup);
}

void mdfa::hopcroft()
{
    QSet<int> accept, notaccepet;
    foreach(auto node,dfaNodes)
    {
        if (node->isAccept())
            accept.insert(node->num);
        else
            notaccepet.insert(node->num);
    }
    T.insert(accept);
    T.insert(notaccepet);
    qDebug() << "origin:" << T;
    while (P != T)
    {
        P = T;
        T.clear();
        qDebug() << P;
        foreach(auto subGroup,P)
        {
            auto result = split(subGroup);
            if(!result.first.isEmpty())
            {
                T.insert(result.first);
            }
            if(!result.second.isEmpty())
            {
                T.insert(result.second);
            }
        }
    }
    qDebug() << "final:" << T;
}

void mdfa::addEdge(int from, int to, QChar ch)
{
    auto f = nodes.value(from);
    auto t = nodes.value(to);
    auto edge = new dfaEdge(ch,f,t,f->edges);
    f->edges = edge;
    edges.insert(ch,edge);
}

void mdfa::parseMDFA()
{
    hopcroft();
    foreach(auto group,T)
    {
        int currentNode = nodeNumber++;
        auto newNode = new dfaNode(currentNode,group);
        foreach(auto node,group)
        {
            if(dfaNodes.value(node)->isAccept())
            {
                newNode->accept = true;
                break;
            }
            else if(node == 0)
            {
                start = currentNode;
                break;
            }
        }
        nodes.insert(currentNode,newNode);
    }
    foreach(auto ch,alphabet)
    {
        auto edges = dfaEdges.values(ch);
        QSet<QPair<int,int>> edgeGroup;
        foreach(auto edge,edges)
        {
            int from=0,to=0;
            foreach(auto node,nodes)
            {
                if(node->closure.contains(edge->from->num))
                {
                    from = node->num;
                }
                if(node->closure.contains(edge->to->num))
                {
                    to = node->num;
                }
            }
            if(!edgeGroup.contains(qMakePair(from,to)))
            {
                edgeGroup.insert(qMakePair(from,to));
                addEdge(from,to,edge->info);
            }
        }
    }
    valid = true;
}

void mdfa::testMDFA(QString str)
{
    auto node = nodes[start];
    foreach(QChar ch,str)
    {
        auto edge=node->edges;
        while(edge != nullptr)
        {
            if(edge->accept(ch))
            {
                qDebug() << QChar('A'+node->num) << "----" << ch << "---->" << QChar('A'+edge->to->num);
                node = edge->to;
                break;
            }
            edge = edge->next;
        }
    }
    qDebug() << (node->isAccept() ? "ACCEPT" : "REJECT");
}

void mdfa::toPrintable()
{
    qDebug() << "start node:" << start;
    foreach(auto node,nodes)
    {
        auto edge=node->edges;
        while(edge != nullptr)
        {
            qDebug() << "\t" << edge->from->num << " ---- (" << edge->info << ") ---- " << edge->to->num << (edge->to->isAccept() ? "ACCEPT" : "");
            edge = edge->next;
        }
    }
}

bool mdfa::match(const QString &str)
{
    auto node = nodes[start];
    foreach(QChar ch,str)
    {
        auto edge=node->edges;
        while(edge != nullptr)
        {
            if(edge->accept(ch))
            {
                node = edge->to;
                break;
            }
            edge = edge->next;
        }
    }
    return node->isAccept();
}

