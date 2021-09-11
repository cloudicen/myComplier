//
// Created by lk on 2021/9/10.
//

#include "NFA.h"

const regEngine::NFA_Graph& regEngine::NFA::constructGraph() {
    Thompson(const_cast<regEngine::RegNode *>(this->regTree.generateRegTree()));
    LOGGER->debug("NFA Graph:\n" + this->nfaGraph.toPrintable());
    return this->nfaGraph;
}

void regEngine::NFA::Thompson(regEngine::RegNode *subTree) {
    static nfaNode* curStartNode = this->nfaGraph.addNode(NFA_START);
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

        auto newEndNode = this->nfaGraph.addNode(NFA_ACCEPT);
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

        auto newEndNode = this->nfaGraph.addNode(NFA_ACCEPT);

        this->nfaGraph.addEdge(oldStartNode1,newEndNode);
        this->nfaGraph.addEdge(curStartNode,newEndNode);
        this->nfaGraph.addEdge(curStartNode,newStartNode);
        curStartNode = newEndNode;
    }
    else if(op == Element)
    {
        LOGGER->debug("Parse Element Node");
        auto newStartNode = this->nfaGraph.addNode(NFA_ACCEPT);
        this->nfaGraph.addEdge(curStartNode,newStartNode,subTree->info);
        curStartNode = newStartNode;
    }
}

int regEngine::NFA::match(const std::string &str) {
    int curMatchPos = 0;
    auto curMatchChar = [&curMatchPos,&str](){return (curMatchPos < str.size() ? str.at(curMatchPos) : '#');};
    auto beginNode = this->nfaGraph.getStartNode();

    auto curState = this->nfaGraph.getEpsClosure({beginNode});

    for(;curMatchPos < str.size();curMatchPos++) {
        //get smove
        auto sMove = this->nfaGraph.getsMove(std::move(curState),curMatchChar());

        //get nextState
        curState = this->nfaGraph.getEpsClosure(std::move(sMove));

        //no next state, break
        if (curState.empty()) {
            break;
        }
    }

    auto acceptNode = this->nfaGraph.getAcceptNode();

    std::stringstream printString;
    if (curMatchPos == str.size() && curState.contains(acceptNode)){
        printString << "NFA_ACCEPT ALL.";
    } else {
        printString << "REJECT (last match char: " << curMatchChar() << ",pos: " << curMatchPos << ")";
    }
    LOGGER->debug(printString.str());

    return curMatchPos;
}
