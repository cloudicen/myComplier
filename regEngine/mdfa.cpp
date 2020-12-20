#include "mdfa.h"

QPair<QSet<int>, QSet<int> > mdfa::split(QSet<int> nodeGroup)
{
    for (auto c = alphabet.begin(); c != alphabet.end(); ++c)// 遍历字符表
    {
        QSet<int> s1, s2;// 存储新的分组
        foreach(auto node,nodeGroup)// 遍历组内节点
        {
            for(auto edge=dfaNodes.value(node)->edges;edge!=nullptr;edge=edge->next)// 遍历该节点的边
            {
                if(edge->info == *c)// 找出所有接受当前字符的边
                {
                    auto from = edge->from->num;
                    auto to = edge->to->num;
                    qDebug() << "CHECK EDGE:" << from << "----" << *c << "----->" << to;
                    if(nodeGroup.contains(to))// 当前节点指向当前组，放入s1
                    {
                        s1.insert(from);
                    }
                    else// 当前节点指向其他组，放入s2
                    {
                        s2.insert(from);
                    }

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
        bool hasEdge=false;
        while(edge != nullptr)
        {
            if(edge->accept(ch))
            {
                qDebug() << QChar('A'+node->num) << "----" << ch << "---->" << QChar('A'+edge->to->num);
                node = edge->to;
                hasEdge=true;
                break;
            }
            edge = edge->next;
        }
        if(!hasEdge)//对当前输入无状态转移边，直接结束循环
        {
            break;
        }
    }
    qDebug() << (node->isAccept() ? "ACCEPT" : "REJECT");
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
            if(edge->to->num == start)
            {
                to = " START";
            }
            if(edge->to->isAccept())
            {
                to = " ACCEPT";
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

bool mdfa::match(const QString &str)
{
    auto node = nodes[start];
    foreach(QChar ch,str)
    {
        auto edge=node->edges;
        bool hasEdge=false;
        while(edge != nullptr)
        {
            if(edge->accept(ch))
            {
                node = edge->to;
                hasEdge=true;
                break;
            }
            edge = edge->next;
        }
        if(!hasEdge)//对当前输入无状态转移边，直接结束循环
        {
            break;
        }
    }
    return node->isAccept();
}

