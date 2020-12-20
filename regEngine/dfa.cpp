#include "dfa.h"

void dfa::subsetConstruct()
{
    auto beginStates = getEPSclosure({nfaStart});
    qDebug() << "Start State closure" << beginStates;
    QVector<QSet<int>> worklist;
    worklist.push_back(beginStates);
    while (!worklist.empty())
    {
        auto currentStates = worklist.front();
        worklist.pop_front();
        foreach(auto ch,alphabet)
        {
            QSet<int> smove = getSMove(currentStates,ch);
            auto nextState = getEPSclosure(smove);
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

void dfa::addEdge(QSet<int> from, QSet<int> to, QChar ch)
{
    dfaNode* f = getNode(from);
    dfaNode* t = getNode(to);
    dfaEdge* edge = new dfaEdge(ch,f,t,f->edges);
    f->edges = edge;
    edges.insert(ch,edge);
}

dfaNode *dfa::getNode(QSet<int> closure)
{
    foreach(auto node,nodes)
    {
        if(node->closure == closure)
        {
            return node;
        }
    }
    dfaNode* newNode = new dfaNode(nodeNumber,closure);
    if(closure.contains(nfaAccept))
    {
        newNode->accept = true;
    }
    if(nodes.isEmpty())
    {
        start=nodeNumber;
    }
    nodes.insert(nodeNumber,newNode);
    nodeNumber++;
    needNewNode=true;
    return newNode;
}

void dfa::parseAlphabet()
{
    foreach(auto edge,nfaEdges.keys())
    {
        QString info = edge;
        if(edge != nfaEdge::EPS)
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

QSet<int> dfa::getEPSclosure(QSet<int> start)
{
    QSet<int> closure;
    closure = start;
    foreach(auto n,start)
    {
        QVector<int> nextNodes;
        nextNodes.push_back(n);

        while (!nextNodes.isEmpty())
        {
            auto node = nfaNodes.find(nextNodes.back()).value();
            nextNodes.pop_back();
            for(auto edge=node->edges;edge!=nullptr;edge = edge->next)
            {
                if(edge->isEPS())
                {
                    if(!closure.contains(edge->to->num))
                    {
                        closure.insert(edge->to->num);
                        nextNodes.push_back(edge->to->num);
                    }
                }
            }
        }
    }
    return closure;
}

QSet<int> dfa::getSMove(QSet<int> current, QChar ch)
{
    QSet<int> closure;
    foreach(auto n,current)
    {
        auto node = nfaNodes.value(n);
        for(auto edge=node->edges;edge!=nullptr;edge = edge->next)
        {
            if(edge->accept(ch))
            {
                if(!closure.contains(edge->to->num))
                {
                    closure.insert(edge->to->num);
                }
            }
        }
    }
    return closure;
}

void dfa::parseDFA()
{
    parseAlphabet();
    subsetConstruct();
}

void dfa::testDFA(QString str)
{
    auto node = nodes[start];
    int index = 0;
    foreach(QChar ch,str)
    {
        auto edge=node->edges;
        bool hasEdge = false;
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
        if(!hasEdge)//对当前输入无状态转移边，直接结束循环
        {
            qDebug() << "dfa process terminate.";
            break;
        }
        index++;
    }
    qDebug() << (node->isAccept()&&index == str.size() ? "ACCEPT" : "REJECT");
}

void dfa::toPrintable()
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
