#include "mdfa.h"

QPair<QSet<int>, QSet<int> > mdfa::split(QSet<int> nodeGroup)
{
    foreach(auto ch ,alphabet)// 遍历字符表
    {
        QSet<int> s1, s2;// 存储新的分组
        auto edges = DFAgraph.findEdges(ch);
        foreach(auto edge,edges)
        {
            auto from=edge->from.toStrongRef()->num;
            auto to=edge->to.toStrongRef()->num;
            if(nodeGroup.contains(from))
            {
                if(nodeGroup.contains(to))
                {
                    qDebug() << "CHECK EDGE:" << from << "----" << ch << "----->" << to << " => [current group]";
                    s1.insert(from);
                }
                else
                {
                    qDebug() << "CHECK EDGE:" << from << "----" << ch << "----->" << to << " => [other group]";
                    s2.insert(from);
                }
            }
        }
        if (!s2.isEmpty() && !s1.isEmpty())//如果s1,s2均不空，则表明当前分组可以进一步细分，直接返回分组结果
        {
            qDebug() << nodeGroup << "can spilt to:" << s1 << s2;
            return qMakePair(s1,s2);
        }
    }
    //遍历完所有字符，均不可区分
    qDebug() << nodeGroup << "can't spilt";
    return qMakePair(QSet<int>(),QSet<int>());
}

void mdfa::hopcroft()
{
    QSet<int> accept, notaccept;
    foreach(auto node,DFAgraph.getAllNodes())
    {
        if (node->isAccept())
            accept.insert(node->num);
        else
            notaccept.insert(node->num);
    }

    QSet<QSet<int>> currentGroup={
        accept,
        notaccept
    };
    qDebug() << "origin group:" << currentGroup;
    while (!currentGroup.isEmpty())
    {
        auto tempSet = currentGroup;
        currentGroup.clear();
        foreach(auto subGroup,tempSet)
        {
            if(subGroup.size() > 1)//节点有多个元素，尝试分割
            {
                auto result = split(subGroup);
                if(!result.first.isEmpty() && !result.second.isEmpty())//节点可分
                {
                    currentGroup.insert(result.first);
                    currentGroup.insert(result.second);
                }
                else// 节点不可再分，加入最终组集合
                {
                    finalGroups.insert(subGroup);
                }
            }
            else// 节点组为空或者只有一个元素，不可再分
            {
                qDebug() << subGroup << "can't spilt";
                if(!subGroup.isEmpty())// 将非空组加入最终组集合
                {
                    finalGroups.insert(subGroup);
                }
            }
        }
    }
    qDebug() << "final group:" << finalGroups;
}

void mdfa::addEdge(int from, int to, QChar ch)
{
    auto f = nodes.value(from);
    auto t = nodes.value(to);
    auto edge = QSharedPointer<dfaEdge>::create(ch,f,t);
    f->edges[ch] = edge;
    edges.insert(ch,edge);
}

void mdfa::parseMDFA()
{
    hopcroft();
    foreach(auto group,finalGroups)
    {
        int currentNode = nodeNumber++;
        auto newNode = QSharedPointer<dfaNode>::create(currentNode,group);
        foreach(auto node,group)
        {
            if(node == 0)
            {
                start = currentNode;
                newNode->start=true;
            }
            if(DFAgraph.findNode(node)->isAccept())
            {
                newNode->accept = true;
            }
            else if(DFAgraph.findNode(node)->isDead())
            {
                newNode->dead = true;
            }
        }
        nodes.insert(currentNode,newNode);
    }
    foreach(auto ch,alphabet)
    {
        auto edges = DFAgraph.findEdges(ch);
        QSet<QPair<int,int>> edgeGroup;
        foreach(auto edge,edges)
        {
            int from=0,to=0;
            foreach(auto node,nodes)
            {
                if(node->closure.contains(edge->from.toStrongRef()->num))
                {
                    from = node->num;
                }
                if(node->closure.contains(edge->to.toStrongRef()->num))
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
    bool hasEdge = false;
    int lastMatchPos=-1;
    int pos=0;
    foreach(QChar ch,str)
    {
        auto edge=node->edges.value(ch);
        hasEdge = false;
        if(!edge.isNull())
        {
            qDebug() << node->num << "----" << ch << "---->" << edge->to.toStrongRef()->num;
            node = edge->to;
            hasEdge = true;
            if(node->isAccept())
            {
                lastMatchPos=pos;
            }
        }
        if(node->isDead() || !hasEdge)//对当前输入无状态转移边，直接结束循环
        {
            if(lastMatchPos != -1)
            {
                qDebug() << "mdfa search process terminate with a match at pos:" << lastMatchPos;
            }
            else
            {
                qDebug() << "mdfa search process terminate with no matches";
            }
            break;
        }
        pos++;
    }
    if(node->isAccept()&&hasEdge)
    {
        qDebug() << "ACCEPT" << "-> index:" << lastMatchPos << "(all accept)";
    }
    else if(lastMatchPos != -1)
    {
        qDebug() << "ACCEPT" << "-> index:" << lastMatchPos << "(partly accept)";
    }
    else
    {
        qDebug() << "REJECT";
    }
}

void mdfa::print()
{
    std::cout << "------------------------------------------------------\n";
    std::cout << "| "<< "start node:" << start << " | " << "total node count:" << nodeNumber << ".\n";
    std::cout << "|\n";
    foreach(auto node,nodes)
    {
        auto edges=node->edges;
        foreach(auto edge,edges)
        {
            QString info;
            auto p = QDebug(&info);
            p.setAutoInsertSpaces(false);
            p << edge->info;
            std::cout << "| " << edge->toPrintable().toStdString() << "\n";
        }
    }
    std::cout << "|\n";
    std::cout << "------------------------------------------------------\n";
}

int mdfa::match(const QString &str)
{
    auto node = nodes[start];
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

