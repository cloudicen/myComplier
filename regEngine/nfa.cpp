#include "nfa.h"

void nfa::Thompson(const regNode *subTree)
{
    auto op = subTree->type;
    if(op == Alternate)
    {
        Thompson(subTree->children[0]);
        int start1 = start;
        int accept1 = accept;
        Thompson(subTree->children[1]);
        int start2 = start;
        int accept2 = accept;
        int from = nodeNumber++;
        addEdge(from, start1, nfaEdge::EPS);
        addEdge(from, start2, nfaEdge::EPS);
        int to = nodeNumber++;
        addEdge(accept1, to, nfaEdge::EPS);
        addEdge(accept2, to, nfaEdge::EPS);
        start = from;
        accept = to;
    }
    else if(op == Concat)
    {
        Thompson(subTree->children[0]);
        int oldStart = start;
        nodeNumber--;
        int oldAccept = accept;
        Thompson(subTree->children[1]);
        addEdge(oldAccept, start, nfaEdge::EPS);
        start = oldStart;
    }
    else if(op == Closure)
    {
        Thompson(subTree->children[0]);
        addEdge(accept, start, nfaEdge::EPS);
        int from = nodeNumber++;
        addEdge(from, start, nfaEdge::EPS);
        int to = nodeNumber++;
        addEdge(from, to, nfaEdge::EPS);
        addEdge(accept, to, nfaEdge::EPS);
        start = from;
        accept = to;
    }
    else if(op ==Element)
    {
        int from = nodeNumber++;
        int to = nodeNumber++;
        addEdge(from, to, subTree->info);
        start = from;
        accept = to;
    }
}

void nfa::addEdge(int from, int to, QString info)
{
    nfaNode *f = getNode(from);
    nfaNode *t = getNode(to);
    nfaEdge *edge = new nfaEdge(info, f, t, f->edges);
    f->edges = edge;
    if(info != nfaEdge::EPS)
    {
        edges.insert(info,edge);
    }
}

nfaNode *nfa::getNode(int num)
{
    auto node = nodes.find(num);
    if(node != nodes.end())
    {
        return node.value();
    }
    else
    {
        nfaNode *p = new nfaNode(num);
        nodes[num] = p;
        return p;
    }
}

void nfa::parseNFA()
{
    Thompson(regTreeRoot);
    valid = true;
}

void nfa::testNFA(QString str="")
{
    nfaNode* node = nullptr;
    nfaEdge* edge = nullptr;

    QVector<int> nextNodes;
    nextNodes.push_back(start);

    QVector<QSet<int>> closures;

    //EPS closure
    QSet<int> EPSclosure;
    EPSclosure.insert(start);

    while (!nextNodes.isEmpty())
    {
        node = getNode(nextNodes.back());
        nextNodes.pop_back();
        edge = node->edges;
        while(edge != nullptr)
        {
            if(edge->isEPS())
            {
                if(!EPSclosure.contains(edge->to->num))
                {
                    EPSclosure.insert(edge->to->num);
                    nextNodes.push_back(edge->to->num);
                }
            }
            edge = edge->next;
        }
    }
    closures.push_back(EPSclosure);

    for(int i=0;i<str.length();i++)
    {
        nfaNode* node = nullptr;
        nfaEdge* edge = nullptr;
        QSet<int> closure;
        foreach(auto n,closures.back())
        {
            QChar ch(str.at(i));
            QVector<int> nextNodes;
            nextNodes.push_back(n);
            while (!nextNodes.isEmpty())
            {
                node = getNode(nextNodes.back());
                nextNodes.pop_back();
                edge = node->edges;
                while(edge != nullptr)
                {
                    if(edge->accept(ch))
                    {
                        if(!closure.contains(edge->to->num))
                        {
                            closure.insert(edge->to->num);
                            nextNodes.push_back(edge->to->num);
                        }
                    }
                    edge = edge->next;
                }
            }
        }
        if(!closure.isEmpty())//若下一状态集为空，则当前状态集就是一个终态或者是死状态
        {
            closures.push_back(closure);
        }
        else
        {
            if(closures.back().contains(accept))
            {
            }
            else
            {
                //死状态，不可能继续匹配
                break;
            }
        }
    }

    int index=0;
    foreach(auto c,closures)
    {
        if(index == 0)
        {
            qDebug() << "EPS closure" << c << "(\'A\')";
        }
        else
        {
            qDebug() << "smove(" << QChar('A'+index-1) << ',' << str.at(index-1) << ')' << "=" << QChar('A'+index) << c ;
        }
        index++;
    }

    if(!closures.isEmpty() && closures.back().contains(accept))
    {
        qDebug() << "ACCEPT!";
    }
    else
    {
        qDebug() << "REJECT!";
    }
}

void nfa::print()
{
    std::cout << "------------------------------------------------------\n";
    std::cout << "| "<< "start node:" << start  << " | " << "accept node:" << accept << " | " << "total node count:" << nodeNumber << ".\n";
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
            if(edge->from->num == accept)
            {
                from = " ACCEPT  ";
            }
            if(edge->to->num == start)
            {
                to = " START";
            }
            if(edge->to->num == accept)
            {
                to = " ACCEPT";
            }
            std::cout << "| " << from.toStdString() << edge->from->num << " ---- ( " << edge->info.toStdString() << " ) ---- " << edge->to->num << to.toStdString() << "\n";
            edge = edge->next;
        }
    }
    std::cout << "|\n";
    std::cout << "------------------------------------------------------\n";
}
