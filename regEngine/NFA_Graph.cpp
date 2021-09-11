//
// Created by lk on 2021/9/10.
//

#include "NFA_Graph.h"

const std::string regEngine::nfaEdge::EPS;

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
        auto pt = new nfaEdge(fromNode, toNode, info);
        if (info == regEngine::nfaEdge::EPS) {
            this->epsEdges.emplace(pt);
            fromNode->epsEdges.insert(pt);
        } else {
            this->edges.emplace(pt);
            fromNode->edges.insert(std::make_pair(info, pt));
        }
        return pt;
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

std::set<int> regEngine::NFA_Graph::getEpsClosure(std::set<int>&& start) const {
    auto closure = start;
    for (auto node : start) {
        std::stack<const nfaNode *> searchStack;
        searchStack.push(getNode(node));
        while (!searchStack.empty()) {
            auto curNode = searchStack.top();
            searchStack.pop();
            auto epsMove = curNode->getEpsMove();
            for (auto nodeToSearch : epsMove) {
                if (!closure.contains(nodeToSearch->number)) {
                    closure.insert(nodeToSearch->number);
                    searchStack.push(nodeToSearch);
                }
            }
        }
    }
    return closure;
}

std::set<int> regEngine::NFA_Graph::getsMove(std::set<int> &&curState,char curMatchChar) const {
    std::set<int> sMove;
    for (auto nodeNum : curState) {
        auto smove = getNode(nodeNum)->getsMove(curMatchChar);
        for (auto node : smove) {
            sMove.insert(node->number);
        }
    }
    return sMove;
}

std::string regEngine::NFA_Graph::toPrintable() const {
    std::stringstream printString;
    for (const auto &edge : edges) {
        if (edge->fromNode->number == startNodeNumber) {
            printString << "(start)  ";
        } else if (edge->fromNode->number == acceptNodeNumber) {
            printString << "(accept) ";
        } else {
            printString << "(normal) ";
        }

        printString << edge->toPrintable();

        if (edge->toNode->number == startNodeNumber) {
            printString << "  (start)";
        } else if (edge->toNode->number == acceptNodeNumber) {
            printString << " (accept)";
        } else {
            printString << " (normal)";
        }
        printString << "\n";
    }
    for (const auto &edge : epsEdges) {
        if (edge->fromNode->number == startNodeNumber) {
            printString << "(start)  ";
        } else if (edge->fromNode->number == acceptNodeNumber) {
            printString << "(accept) ";
        } else {
            printString << "(normal) ";
        }

        printString << edge->toPrintable();

        if (edge->toNode->number == startNodeNumber) {
            printString << "  (start)";
        } else if (edge->toNode->number == acceptNodeNumber) {
            printString << " (accept)";
        } else {
            printString << " (normal)";
        }
        printString << "\n";
    }
    return printString.str();
}

std::set<const regEngine::nfaEdge *> regEngine::NFA_Graph::getAllEdges() const{
    std::set<const nfaEdge *> result;
    for (const auto & edge : this->edges) {
        result.insert(edge.get());
    }
    return result;
}
