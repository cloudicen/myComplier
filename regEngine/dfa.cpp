#include "dfa.h"

void dfa::subsetConstruct()
{
    auto beginStates = NFAgraph.getEPSclosure({nfaStart});
    qDebug() << "Start State closure" << beginStates;
    QVector<QSet<int>> worklist;
    worklist.push_back(beginStates);
    while (!worklist.empty())
    {
        auto currentStates = worklist.front();
        worklist.pop_front();
        foreach(auto ch,alphabet)
        {
            QSet<int> smove = NFAgraph.getSMove(currentStates,ch);
            auto nextState = NFAgraph.getEPSclosure(smove);
            //如果下一状态集为空，则当前状态已经是终态或者是死状态,这里直接引入死状态
            {
                addEdge(currentStates, nextState, ch);
                qDebug()  << currentStates << '(' << QString('A'+getNode(currentStates)->num) << ')' << "----" << ch << "---->" << nextState << "....." << QString('A'+getNode(nextState)->num);
                if (needNewNode == true)
                {
                    worklist.push_back(nextState);
                    needNewNode = false;
                }
            }
        }
    }
}

void dfa::addEdge(QSet<int> &from, QSet<int> &to, QChar ch)
{
    auto f = getNode(from);
    auto t = getNode(to);
    QSharedPointer<dfaEdge> edge = QSharedPointer<dfaEdge>::create(ch,f,t);
    f->edges[ch] = edge;
    edges.insert(ch,edge);
}

QSharedPointer<dfaNode> dfa::getNode(QSet<int> closure)
{
    foreach(auto node,nodes)
    {
        if(node->closure == closure)
        {
            return node;
        }
    }
    auto newNode = QSharedPointer<dfaNode>::create(nodeNumber,std::move(closure));
    if(closure.isEmpty())
    {
        newNode->dead = true;
    }
    else if(closure.contains(nfaAccept))
    {
        newNode->accept = true;
    }
    if(nodes.isEmpty())
    {
        start=nodeNumber;
        newNode->start=true;
    }
    nodes.insert(nodeNumber,newNode);
    nodeNumber++;
    needNewNode=true;
    return newNode;
}

void dfa::parseAlphabet()
{
    foreach(auto edge,NFAgraph.getAllEdges())
    {
        QString info = edge->info;
        if(!edge->isEPS())
        {
            if(info == "alpha")
            {
                QString str="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
                foreach(QChar c,str)
                {
                    alphabet.insert(c);
                }
            }
            else if(info == "digit")
            {
                QString str="1234567890";
                foreach(QChar c,str)
                {
                    alphabet.insert(c);
                }
            }
            else if(info == "space")
            {
                QString str="\t\n \r";
                foreach(QChar c,str)
                {
                    alphabet.insert(c);
                }
            }
            else if(info == "word")
            {
                QString str="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
                str += "1234567890";
                str += "_";
                foreach(QChar c,str)
                {
                    alphabet.insert(c);
                }
            }
            else if(info == "newline")
            {
                alphabet.insert('\n');
            }
            else if(info == "tab")
            {
                alphabet.insert('\t');
            }
            else if(info == "return")
            {
                alphabet.insert('\r');
            }
            else
            {
                alphabet.insert(info.at(0));
            }
        }
    }
    qDebug() << "alphabet for dfa" << alphabet;
}

void dfa::parseDFA()
{
    parseAlphabet();
    subsetConstruct();
}

void dfa::testDFA(QString str)
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
                qDebug() << "dfa search process terminate with a match at pos:" << lastMatchPos;
            }
            else
            {
                qDebug() << "dfa search process terminate with no matches";
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

void dfa::toPrintable()
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
