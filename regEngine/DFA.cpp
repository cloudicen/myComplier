//
// Created by lk on 2021/9/12.
//

#include "DFA.h"

void regEngine::DFA::constructGraph() {
    const auto& graph = this->nfa.constructGraph();
    genTokenSet(graph);
    subSetMethod(graph);
    LOGGER->debug("DFA Graph:\n" + this->dfaGraph.toPrintable());
}

int regEngine::DFA::match(const std::string &str) {
    return 0;
}

void regEngine::DFA::genTokenSet(const NFA_Graph &graph) {
    for (auto edge : graph.getAllEdges()) {
        if (edge->info.size() > 1) {
            //rich info
            assert(regEngine::info2TokenDef.contains(edge->info));
            auto tokens = regEngine::info2TokenDef[edge->info];
            for (auto token : tokens) {
                this->tokenSet.insert(token);
            }
        } else {
            //basic char
            this->tokenSet.insert(edge->info.at(0));
        }
    }
}

void regEngine::DFA::subSetMethod(const NFA_Graph &graph) {
    auto nfaStartNode = graph.getStartNode();
    auto nfaAcceptNode = graph.getAcceptNode();

    auto beginStates = graph.getEpsClosure({nfaStartNode->number});

    auto startNode = this->dfaGraph.addNode(DFA_START);
    auto deadNode = this->dfaGraph.addNode(DFA_DEAD);

    std::map<std::set<int>,DFA_Node*> stateSet;
    stateSet.insert(std::make_pair(beginStates,startNode));
    stateSet.insert(std::make_pair(std::set<int>(),deadNode));

    std::stack<std::set<int>> stateStack;
    stateStack.push(beginStates);

    while (!stateStack.empty())
    {
        auto curStates = stateStack.top();
        stateStack.pop();
        auto curNode = stateSet.at(curStates);
        for (auto ch:this->tokenSet){
            auto currentStates = curStates;
            auto sMove = graph.getsMove(std::move(currentStates), ch);
            auto nextStates = graph.getEpsClosure(std::move(sMove));

            if (nextStates.empty()) {
                dfaGraph.addEdge(curNode,deadNode,ch);
            } else {
                if (!stateSet.contains(nextStates)) {
                    DFA_Node *newNode;
                    if (nextStates.contains(nfaAcceptNode->number)) {
                        //add accept node
                        newNode = dfaGraph.addNode(DFA_ACCEPT);
                    } else {
                        //add normal node
                        newNode = dfaGraph.addNode(DFA_NORMAL);
                    }
                    //add to work set
                    stateSet.insert(std::make_pair(nextStates,newNode));
                    stateStack.push(nextStates);
                    dfaGraph.addEdge(curNode,newNode,ch);
                } else {
                    auto node = stateSet.at(nextStates);
                    dfaGraph.addEdge(curNode,node,ch);
                }
            }
        }
    }
}
