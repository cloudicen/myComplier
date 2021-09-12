//
// Created by lk on 2021/9/12.
//

#include "DFA.h"

const regEngine::DFA_Graph &regEngine::DFA::constructGraph() {
    const auto &graph = this->nfa.constructGraph();
    genTokenSet(graph);
    subSetMethod(graph);
    LOGGER->debug("DFA Graph:\n" + this->dfaGraph.toPrintable());
    return this->dfaGraph;
}

const regEngine::DFA_Graph &regEngine::DFA::minimizeGraph() {
    hopcroft();
    LOGGER->debug("mDFA Graph:\n" + this->dfaGraph.toPrintable());
    return this->dfaGraph;
}

int regEngine::DFA::match(const std::string &str) {
    int curMatchPos = 0;
    auto curMatchChar = [&curMatchPos,&str](){return (curMatchPos < str.size() ? str.at(curMatchPos) : '#');};
    auto beginNode = this->dfaGraph.getStartNode();

    auto curState = beginNode;

    for(;curMatchPos < str.size();curMatchPos++) {
        auto sMove = curState->getsMove(curMatchChar());
        curState = this->dfaGraph.getNode(sMove);

        if (curState == this->dfaGraph.getDeadNode()){
            break;
        }
    }

    auto acceptState = this->dfaGraph.getAcceptNode();

    std::stringstream printString;
    if (curMatchPos == str.size() && acceptState.contains(curState)){
        printString << "ACCEPT ALL.";
    } else {
        printString << "REJECT (last match char: " << curMatchChar() << ",pos: " << curMatchPos << ")";
    }
    LOGGER->debug(printString.str());

    return curMatchPos;
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

    std::map<std::set<int>, DFA_Node *> stateSet;
    stateSet.insert(std::make_pair(beginStates, startNode));
    stateSet.insert(std::make_pair(std::set<int>(), deadNode));

    std::stack<std::set<int>> workStack;
    workStack.push(beginStates);

    while (!workStack.empty()) {
        auto curStates = workStack.top();
        workStack.pop();
        auto curNode = stateSet.at(curStates);
        for (auto ch:this->tokenSet) {
            auto currentStates = curStates;
            auto sMove = graph.getsMove(std::move(currentStates), ch);
            auto nextStates = graph.getEpsClosure(std::move(sMove));

            if (nextStates.empty()) {
                dfaGraph.addEdge(curNode, deadNode, ch);
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
                    stateSet.insert(std::make_pair(nextStates, newNode));
                    workStack.push(nextStates);
                    dfaGraph.addEdge(curNode, newNode, ch);
                } else {
                    auto node = stateSet.at(nextStates);
                    dfaGraph.addEdge(curNode, node, ch);
                }
            }
        }
    }
}

std::pair<std::set<int>, std::set<int>> regEngine::DFA::spilt(const std::set<int> &originGroup) {
    for (auto ch : this->tokenSet) {
        std::set<int> group1, group2;
        for (auto nodeNum : originGroup) {
            auto sMove = this->dfaGraph.getNode(nodeNum)->getsMove(ch);
            if (originGroup.contains(sMove)) {
                group1.insert(nodeNum);
            } else {
                group2.insert(nodeNum);
            }
        }
        if (!group1.empty() && !group2.empty()) {
            return std::make_pair(group1, group2);
        }
    }
    return std::make_pair(originGroup, std::set<int>());
}

void regEngine::DFA::hopcroft() {
    std::set<int> accept, notAccept, all;
    all = this->dfaGraph.getAllNodeNumber();
    accept = this->dfaGraph.getAcceptNodeNumber();

    //calculate diff set
    std::set_difference(all.begin(), all.end(), accept.begin(), accept.end(), std::inserter(notAccept, accept.begin()));

    std::set<std::set<int>> currentGroup{accept, notAccept};

    std::set<std::set<int>> finalGroup;

    while (!currentGroup.empty()) {
        auto tempGroup = currentGroup;
        currentGroup.clear();
        for (const auto &subGroup : tempGroup) {
            if (subGroup.size() > 1)//节点有多个元素，尝试分割
            {
                auto[subGroup1, subGroup2] = this->spilt(subGroup);
                if (!subGroup1.empty() && !subGroup2.empty())//节点可分
                {
                    currentGroup.insert(subGroup1);
                    currentGroup.insert(subGroup2);
                } else// 节点不可再分，加入最终组集合
                {
                    finalGroup.insert(subGroup);
                }
            } else// 节点组为空或者只有一个元素，不可再分
            {
                if (!subGroup.empty())// 将非空组加入最终组集合
                {
                    finalGroup.insert(subGroup);
                }
            }
        }
    }

    DFA_Graph mDFA;

    finalGroup.erase({this->dfaGraph.getDeadNodeNumber()});

    std::map<int,DFA_Node*> newNodes;

    for (const auto& group: finalGroup) {
        auto flag = DFA_NORMAL;
        for (auto nodeNum : group) {
            if (this->dfaGraph.getNode(nodeNum)->nodeType == DFA_ACCEPT) {
                flag = DFA_ACCEPT;
                break;
            } else if (this->dfaGraph.getNode(nodeNum)->nodeType == DFA_START) {
                flag = DFA_START;
                break;
            }
        }
        auto newNode = mDFA.addNode(flag);
        //originNode -> newNode
        for (auto originNodeNum : group) {
            newNodes.emplace(std::make_pair(originNodeNum,newNode));
        }
    }

    //use tuple set to make sure there is no duplicate edge
    std::set<std::tuple<DFA_Node*,DFA_Node*,char>> edgeSet;

    for (auto [originNodeNum,newNode] : newNodes) {
        for (auto ch : this->tokenSet) {
            auto sMove = this->dfaGraph.getNode(originNodeNum)->getsMove(ch);
            if (sMove != this->dfaGraph.getDeadNodeNumber() && !edgeSet.contains({newNode,newNodes[sMove],ch})) {
                mDFA.addEdge(newNode,newNodes[sMove],ch);
                edgeSet.insert({newNode,newNodes[sMove],ch});
            }
        }
    }

    this->dfaGraph = std::move(mDFA);
}


