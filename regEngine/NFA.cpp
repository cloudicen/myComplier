//
// Created by lk on 2021/9/10.
//

#include "NFA.h"

void regEngine::NFA::constructGraph() {
    Thompson(const_cast<regEngine::RegNode *>(this->regTree.generateRegTree()));
    printf("%s",this->nfaGraph.toPrintable().c_str());
}

void regEngine::NFA::Thompson(regEngine::RegNode *subTree) {
    static nfaNode* curStartNode = this->nfaGraph.addNode(START);
    if (subTree == nullptr) {
        return;
    }
    auto op = subTree->type;
    if(op == Alternate)
    {
        LOGGER->debug("Parse Alternate Node");
        auto oldStartNode = curStartNode;

        auto newStartNode1 = this->nfaGraph.addNode();
        this->nfaGraph.addEdge(oldStartNode,newStartNode1);
        curStartNode = newStartNode1;
        Thompson(subTree->leftChildNode.get());

        auto newStartNode2 = this->nfaGraph.addNode();
        this->nfaGraph.addEdge(oldStartNode,newStartNode2);
        oldStartNode = curStartNode;

        curStartNode = newStartNode2;
        Thompson(subTree->rightChildNode.get());

        auto newEndNode = this->nfaGraph.addNode(ACCEPT);
        this->nfaGraph.addEdge(oldStartNode,newEndNode);
        this->nfaGraph.addEdge(curStartNode,newEndNode);

        curStartNode = newEndNode;
    }
    else if(op == Concat)
    {
        LOGGER->debug("Parse Concat Node");
        Thompson(subTree->leftChildNode.get());
        Thompson(subTree->rightChildNode.get());
    }
    else if(op == Closure)
    {
        LOGGER->debug("Parse Closure Node");
        while (subTree->leftChildNode->type == Closure) {
            subTree = subTree->leftChildNode.get();
        }
        auto oldStartNode1 = curStartNode;
        auto newStartNode = this->nfaGraph.addNode();
        this->nfaGraph.addEdge(oldStartNode1,newStartNode);
        curStartNode = newStartNode;
        Thompson(subTree->leftChildNode.get());

        auto newEndNode = this->nfaGraph.addNode(ACCEPT);

        this->nfaGraph.addEdge(oldStartNode1,newEndNode);
        this->nfaGraph.addEdge(curStartNode,newEndNode);
        this->nfaGraph.addEdge(curStartNode,newStartNode);
        curStartNode = newEndNode;
    }
    else if(op == Element)
    {
        LOGGER->debug("Parse Element Node");
        auto newStartNode = this->nfaGraph.addNode(ACCEPT);
        this->nfaGraph.addEdge(curStartNode,newStartNode,subTree->info);
        curStartNode = newStartNode;
    }
}
