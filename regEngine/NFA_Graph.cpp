//
// Created by lk on 2021/9/10.
//

#include "NFA_Graph.h"

std::string regEngine::nfaEdge::EPS;

std::string regEngine::nfaEdge::toPrintable() const {
    std::stringstream printString;
    printString << fromNode->toPrintable();
    printString << " -------";
    printString << (this->info == EPS ? "eps" : info);
    printString << "-------> ";
    printString << toNode->toPrintable();
    return printString.str();
}

bool regEngine::nfaEdge::accept(char ch) const {
    return regEngine::matchRules[this->info](ch);
}

std::unordered_set<const regEngine::nfaNode *> regEngine::nfaNode::getEpsClosure() const{
    std::unordered_set<const nfaNode *> result;
    for (auto edge : epsEdges) {
        result.insert(edge->toNode);
    }
    return result;
}

std::unordered_set<const regEngine::nfaNode *> regEngine::nfaNode::getsMove(char ch) const{
    std::unordered_set<const nfaNode *> result;
    for (auto edge : edges) {
        if (edge.second->accept(ch)) {
        result.insert(edge.second->toNode);
        }
    }
    return result;
}

std::string regEngine::nfaNode::toPrintable() const {
    std::stringstream printString;
    printString << "{ " << (number == -1 ? "invalid" : std::to_string(number))
                << " }";
    return printString.str();
}

regEngine::nfaNode  *regEngine::NFA_Graph::addNode(NFA_NodeType type) {
    nodeCount++;
    auto[pt, success] = this->nodes.emplace(std::make_pair(nodeCount, std::make_unique<nfaNode>(nodeCount)));
    if (type == START) {
        startNodeNumber = nodeCount;
    } else if (type == ACCEPT) {
        acceptNodeNumber = nodeCount;
    }
    return pt->second.get();
}

regEngine::nfaEdge *regEngine::NFA_Graph::addEdge(regEngine::nfaNode *fromNode, regEngine::nfaNode *toNode, const std::string &info) {
    if (fromNode != nullptr && toNode != nullptr) {
        auto pt = this->edges.emplace(std::make_pair(info, std::make_unique<nfaEdge>(fromNode, toNode, info)));
        if (info == regEngine::nfaEdge::EPS) {
            fromNode -> epsEdges.insert(pt->second.get());
        } else {
            fromNode -> edges.insert(std::make_pair(info, pt->second.get()));
        }
        return pt->second.get();
    }
    return nullptr;
}

const regEngine::nfaNode *regEngine::NFA_Graph::getNode(int number) const{
    return this->nodes.at(number).get();
}

const regEngine::nfaNode *regEngine::NFA_Graph::getStartNode() const{
    return getNode(this->startNodeNumber);
}

const regEngine::nfaNode *regEngine::NFA_Graph::getAcceptNode() const{
    return getNode(this->acceptNodeNumber);
}
