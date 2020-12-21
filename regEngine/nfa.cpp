#include "nfa.h"

void nfa::Thompson(const QWeakPointer<const regNode> &subTree)
{
    auto treeRef = subTree.toStrongRef();
    if(!treeRef.isNull())
    {
        auto op = treeRef->type;
        if(op == Alternate)
        {
            qDebug() << "Parse Alternate Node";
            Thompson(treeRef->children[0]);
            int start1 = start;
            int accept1 = accept;
            Thompson(treeRef->children[0]);
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
            qDebug() << "Parse Concat Node";
            Thompson(treeRef->children[0]);
            int oldStart = start;
            nodeNumber--;
            //int oldAccept = accept;
            Thompson(treeRef->children[1]);
            //addEdge(oldAccept, start, nfaEdge::EPS);
            start = oldStart;
        }
        else if(op == Closure)
        {
            qDebug() << "Parse Closure Node";
            int from = nodeNumber++;
            Thompson(treeRef->children[0]);
            addEdge(accept, start, nfaEdge::EPS);
            addEdge(from, start, nfaEdge::EPS);
            int to = nodeNumber++;
            addEdge(from, to, nfaEdge::EPS);
            addEdge(accept, to, nfaEdge::EPS);
            start = from;
            accept = to;
        }
        else if(op ==Element)
        {
            qDebug() << "Parse Element Node";
            int from = nodeNumber++;
            int to = nodeNumber++;
            if(treeRef->info=="empty")//如果当前节点代表空串，则添加eps边
            {
                addEdge(from,to,nfaEdge::EPS);
            }
            else
            {
                addEdge(from, to, treeRef->info);
            }
            start = from;
            accept = to;
        }
    }
}

void nfa::addEdge(int from, int to, QString info)
{
    auto f = getNode(from);
    auto t = getNode(to);
    QSharedPointer<nfaEdge> edge = QSharedPointer<nfaEdge>::create(info, f, t);
    if(info != nfaEdge::EPS)
    {
        f->edges.insert(info,edge);
        edges.insert(info,edge);
    }
    else
    {
        f->epsEdges.insert(edge);
    }
}

QSharedPointer<nfaNode> nfa::getNode(int num)
{
    auto node = nodes.find(num);
    if(node != nodes.end())
    {
        return node.value();
    }
    else
    {
        QSharedPointer<nfaNode> p = QSharedPointer<nfaNode>::create(num);
        nodes[num] = p;
        return p;
    }
}

void nfa::parseNFA()
{
    Thompson(tree.getRootNode());
    nodes[start]->type = nfaNodeType::start;
    nodes[accept]->type = nfaNodeType::accept;
    valid = true;
}

void nfa::testNFA(QString str)
{
    QSet<int> currentStates = getEPSclosure({start});

    qDebug() << "begin state" << currentStates;
    int lastMatchPos=-1;
    int pos=0;
    foreach(auto ch,str)
    {
        auto smove = getSMove(currentStates,ch);
        qDebug() << "smove(" << currentStates << ',' << ch << ')' << "=" << smove;
        currentStates = getEPSclosure(smove);
        qDebug() << "next states" << currentStates;
        if(currentStates.contains(accept))
        {
            lastMatchPos=pos;
        }
        if(currentStates.isEmpty())//对当前输入无状态转移边，直接结束循环
        {
            if(lastMatchPos != -1)
            {
                qDebug() << "nfa search process terminate with a match at pos:" << lastMatchPos;
            }
            else
            {
                qDebug() << "nfa search process terminate with no matches";
            }
            break;
        }
        pos++;
    }

    if(currentStates.contains(accept))
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

void nfa::print()
{
    std::cout << "------------------------------------------------------\n";
    std::cout << "| "<< "start node:" << start  << " | " << "accept node:" << accept << " | " << "total node count:" << nodeNumber << ".\n";
    std::cout << "|\n";
    foreach(auto node,nodes)
    {
        auto edges=node->edges;
        foreach(auto edge,edges)
        {
            std::cout << "| " << edge->toPrintable().toStdString() << "\n";
        }
        auto espEdges=node->epsEdges;
        foreach(auto edge,espEdges)
        {
            std::cout << "| " << edge->toPrintable().toStdString() << "\n";
        }
    }
    std::cout << "|\n";
    std::cout << "------------------------------------------------------\n";
}

QSet<int> nfa::getEPSclosure(QSet<int> start)
{
    QSet<int> closure;
    closure = start;
    foreach(auto n,start)
    {
        QVector<int> nextNodes;
        nextNodes.push_back(n);
        while (!nextNodes.isEmpty())
        {
            auto node = nodes.value(nextNodes.back());
            nextNodes.pop_back();
            auto nodeEdges = node->epsEdges;
            foreach(auto edge,nodeEdges)
            {
                auto toNode = edge->to.toStrongRef();
                if(!toNode.isNull())
                {
                    if(!closure.contains(toNode->num))
                    {
                        closure.insert(toNode->num);
                        nextNodes.push_back(toNode->num);
                    }
                }
            }
        }
    }
    return closure;
}

QSet<int> nfa::getSMove(QSet<int> current, QChar ch)
{
    QSet<int> closure;
    foreach(auto n,current)
    {
        auto node = nodes.value(n);
        auto nodeEdges = node->edges;
        foreach(auto edge,nodeEdges)
        {
            if(edge->accept(ch))
            {
                auto toNode = edge->to.toStrongRef();
                if(!toNode.isNull())
                {
                    closure.insert(toNode->num);
                }
            }
        }
    }
    return closure;
}
