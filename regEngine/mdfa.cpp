#include "mdfa.h"

QPair<QSet<int>, QSet<int> > mdfa::split(QSet<int> nodeGroup)
{
    foreach(auto ch ,alphabet)// 遍历字符表
    {
        QSet<int> s1, s2;// 存储新的分组
        auto edges = dfaEdges.values(ch);
        foreach(auto edge,edges)
        {
            auto from=edge->from->num;
            auto to=edge->to->num;
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
        /*
        foreach(auto node,nodeGroup)// 遍历组内节点
        {
            for(auto edge=dfaNodes.value(node)->edges;edge!=nullptr;edge=edge->next)// 遍历该节点的边
            {
                if(edge->accept(ch))// 找出所有接受当前字符的边
                {
                    auto to = edge->to->num;
                    if(nodeGroup.contains(to))// 当前节点指向当前组，放入s1
                    {
                        qDebug() << "CHECK EDGE:" << node << "----" << ch << "----->" << to << " => [current group]";
                        s1.insert(node);
                    }
                    else// 当前节点指向其他组，放入s2
                    {
                        qDebug() << "CHECK EDGE:" << node << "----" << ch << "----->" << to << " => [other group]";
                        s2.insert(node);
                    }
                }
            }
        }
        */
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
    foreach(auto node,dfaNodes)
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
    auto edge = new dfaEdge(ch,f,t,f->edges);
    f->edges = edge;
    edges.insert(ch,edge);
}

void mdfa::parseMDFA()
{
    hopcroft();
    foreach(auto group,finalGroups)
    {
        int currentNode = nodeNumber++;
        auto newNode = new dfaNode(currentNode,group);
        foreach(auto node,group)
        {
            if(node == 0)
            {
                start = currentNode;
            }
            if(dfaNodes.value(node)->isAccept())
            {
                newNode->accept = true;
            }
            else if(dfaNodes.value(node)->isDead())
            {
                newNode->dead = true;
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
    bool hasEdge = false;
    int lastMatchPos=-1;
    int pos=0;
    foreach(QChar ch,str)
    {
        auto edge=node->edges;
        hasEdge = false;
        while(edge != nullptr)
        {
            if(edge->accept(ch))
            {
                qDebug() << node->num << "----" << ch << "---->" << edge->to->num;
                node = edge->to;
                hasEdge = true;
                break;
            }
            edge = edge->next;
        }
        if(node->isAccept())
        {
            lastMatchPos=pos;
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
        auto edge=node->edges;
        while(edge != nullptr)
        {
            QString from="         ",to="";
            if(edge->from->num == start)
            {
                from = "  START  ";
            }
            if(edge->from->isAccept())
            {
                from = " ACCEPT  ";
            }
            if(edge->from->isDead())
            {
                from = " DEAD    ";
            }
            if(edge->to->num == start)
            {
                to = " START";
            }
            if(edge->to->isAccept())
            {
                to = " ACCEPT";
            }
            if(edge->to->isDead())
            {
                to = " DEAD    ";
            }
            QString info;
            auto p = QDebug(&info);
            p.setAutoInsertSpaces(false);
            p << edge->info;
            std::cout << "| " << from.toStdString() << edge->from->num << " ---- ( " << info.toStdString() << " ) ---- " << edge->to->num << to.toStdString() << "\n";
            edge = edge->next;
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
        auto edge=node->edges;
        hasEdge = false;
        while(edge != nullptr)
        {
            if(edge->accept(ch))
            {
                node = edge->to;
                hasEdge = true;
                break;
            }
            edge = edge->next;
        }
        if(node->isAccept())
        {
            lastMatchPos=pos;
        }
        if(node->isDead() || !hasEdge)//对当前输入无状态转移边，直接结束循环
        {
            break;
        }
        pos++;
    }
    return lastMatchPos;
}

