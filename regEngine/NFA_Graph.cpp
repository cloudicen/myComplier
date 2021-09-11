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
    if (this->info.size() > 1) {
        //rich match rule
        return regEngine::matchRules[this->info](ch);
    } else {
        //single char match
        return this->info.at(0) == ch;
    }
}

std::unordered_set<const regEngine::nfaNode *> regEngine::nfaNode::getEpsMove() const {
    std::unordered_set<const nfaNode *> result;
    result.insert(this);
    for (auto edge : epsEdges) {
        result.insert(edge->toNode);
    }
    return result;
}

std::unordered_set<const regEngine::nfaNode *> regEngine::nfaNode::getsMove(char ch) const {
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

regEngine::nfaNode *regEngine::NFA_Graph::addNode(NFA_NodeType type) {
    nodeCount++;
    auto[pt, success] = this->nodes.emplace(std::make_pair(nodeCount, std::make_unique<nfaNode>(nodeCount)));
    if (type == NFA_START) {
        startNodeNumber = nodeCount;
    } else if (type == NFA_ACCEPT) {
        acceptNodeNumber = nodeCount;
    }
    return pt->second.get();
}

regEngine::nfaEdge *regEngine::NFA_Graph::addEdge(regEngine::nfaNode *fromNode, regEngine::nfaNode *toNode, const std::string &info) {
    if (fromNode != nullptr && toNode != nullptr) {
        auto pt = this->edges.emplace(std::make_pair(info, std::make_unique<nfaEdge>(fromNode, toNode, info)));
        if (info == regEngine::nfaEdge::EPS) {
            fromNode->epsEdges.insert(pt->second.get());
        } else {
            fromNode->edges.insert(std::make_pair(info, pt->second.get()));
        }
        return pt->second.get();
    }
    return nullptr;
}

const regEngine::nfaNode *regEngine::NFA_Graph::getNode(int number) const {
    return this->nodes.at(number).get();
}

const regEngine::nfaNode *regEngine::NFA_Graph::getStartNode() const {
    return getNode(this->startNodeNumber);
}

const regEngine::nfaNode *regEngine::NFA_Graph::getAcceptNode() const {
    return getNode(this->acceptNodeNumber);
}

std::unordered_set<const regEngine::nfaNode *> regEngine::NFA_Graph::getEpsClosure(std::unordered_set<const regEngine::nfaNode *>&& start) const {
    auto closure = start;
    for (auto node : start) {
        std::stack<const nfaNode *> searchStack;
        searchStack.push(node);
        while (!searchStack.empty()) {
            auto curNode = searchStack.top();
            searchStack.pop();
            auto epsMove = curNode->getEpsMove();
            for (auto nodeToSearch : epsMove) {
                if (!closure.contains(nodeToSearch)) {
                    closure.insert(nodeToSearch);
                    searchStack.push(nodeToSearch);
                }
            }
        }
    }
    return closure;
}

std::unordered_set<const regEngine::nfaNode *> regEngine::NFA_Graph::getsMove(std::unordered_set<const nfaNode *> &&curState,char curMatchChar) const {
    std::unordered_set<const nfaNode *> sMove;
    for (auto node : curState) {
        auto smove = node->getsMove(curMatchChar);
        sMove.insert(smove.begin(),smove.end());
    }
    return sMove;
}

std::string regEngine::NFA_Graph::toPrintable() const {
    std::stringstream printString;
    for (const auto &edge : edges) {
        if (edge.second->fromNode->number == startNodeNumber) {
            printString << "(start)  ";
        } else if (edge.second->fromNode->number == acceptNodeNumber) {
            printString << "(accept) ";
        } else {
            printString << "(normal) ";
        }

        printString << edge.second->toPrintable();

        if (edge.second->toNode->number == startNodeNumber) {
            printString << "  (start)";
        } else if (edge.second->toNode->number == acceptNodeNumber) {
            printString << " (accept)";
        } else {
            printString << " (normal)";
        }
        printString << "\n";
    }
    return printString.str();
}
