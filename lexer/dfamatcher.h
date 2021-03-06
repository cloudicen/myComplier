﻿#ifndef DFAMATCHER_H
#define DFAMATCHER_H

#include "../regEngine/mdfa.h"

class dfaMatcher
{
private:
    QWeakPointer<dfaNode> startNode;
    QMap<int,QSharedPointer<dfaNode>> nodes;
    QMultiMap<QChar,QSharedPointer<dfaEdge>> edges;
    bool valid=false;
public:
    dfaMatcher()=default;
    dfaMatcher(const QString&regExpr)
    {
        auto reg = regTree(regExpr);
        if(reg.analyze())
        {
            auto nfaGraph = nfa(reg);
            nfaGraph.parseNFA();
            auto dfaGraph = dfa(nfaGraph);
            dfaGraph.parseDFA();
            auto mdfaGrah = mdfa(dfaGraph);
            mdfaGrah.parseMDFA();
            nodes = mdfaGrah.takeAllNodes();
            edges = mdfaGrah.takeAllEdges();
            startNode = nodes[mdfaGrah.getStartNode()];
            valid=true;
        }
        else
        {
            qWarning() << "reg syntax error";
        }
    };
    dfaMatcher(const dfaMatcher&)=delete;
    dfaMatcher(dfaMatcher&&other)
    {
        nodes=std::move(other.nodes);
        edges=std::move(other.edges);
        startNode=other.startNode;
        valid=other.valid;
        other.startNode.clear();
        other.valid=false;
    }
    bool isValid()
    {
        return valid;
    }

    int match(const QString & str);
};

#endif // DFAMATCHER_H
