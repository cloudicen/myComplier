#include "dfa.h"

void dfa::subsetConstruct()
{
    auto q0 = getEPSclosure({nfaStart});
    qDebug() << "Start State closure" << q0;
    QVector<QSet<int>> worklist;
    worklist.push_back(q0);
    while (!worklist.empty())
    {
        auto q = worklist.front();
        worklist.pop_front();
        for (auto c = alphabet.begin(); c != alphabet.end(); ++c)
        {
            QSet<int> v = getNextClosure(q, *c);
            if(!v.isEmpty())//下一状态集不为空
            {
                addEdge(q, v, *c);
                qDebug()  << q << '(' << QString('A'+getNode(q)->num) << ')' << "----" << *c << "---->" << v << "....." << QString('A'+getNode(v)->num);
                if (needNewNode == true)
                {
                    worklist.push_back(v);
                    needNewNode = false;
                }
            }
            else//如果下一状态集为空，则当前状态已经是终态或者是死状态
            {
                if(q.contains(nfaAccept))
                {
                    //终态
                }
                else
                {
                    //死状态处理，这里忽略死状态转换
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
    nfaNode* node = nullptr;
    nfaEdge* edge = nullptr;
    QSet<int> closure;
    closure = start;
    foreach(auto n,start)
    {
        QVector<int> nextNodes;
        nextNodes.push_back(n);

        while (!nextNodes.isEmpty())
        {
            node = nfaNodes.find(nextNodes.back()).value();
            nextNodes.pop_back();
            edge = node->edges;
            while(edge != nullptr)
            {
                if(edge->isEPS())
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
    return closure;
}

QSet<int> dfa::getNextClosure(QSet<int> current, QChar ch)
{
    nfaNode* node = nullptr;
    nfaEdge* edge = nullptr;
    QSet<int> closure;
    foreach(auto n,current)
    {
        QVector<int> nextNodes;
        nextNodes.push_back(n);
        QChar c = ch;
        while (!nextNodes.isEmpty())
        {
            node = nfaNodes.find(nextNodes.back()).value();
            nextNodes.pop_back();
            edge = node->edges;
            while(edge != nullptr)
            {
               if(edge->accept(c))
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

void dfa::toPrintable()
{
    qDebug() << "dfa begin node:" << start;
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
