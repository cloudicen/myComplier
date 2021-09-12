//
// Created by lk on 2021/9/11.
//

#include "DFA_Graph.h"

std::string regEngine::DFA_Edge::toPrintable() const {
    std::stringstream printString;
    printString << fromNode->toPrintable();
    printString << " -------";
    if (this->info == 0) {
        printString << "empty";
    } else {
        printString << info;
    }
    printString << "-------> ";
    printString << toNode->toPrintable();
    return printString.str();
}

std::string regEngine::DFA_Node::toPrintable() const {
    std::stringstream printString;
    printString << "{ " << (number == -1 ? "invalid" : std::to_string(number))
                << " }";
    return printString.str();
}


regEngine::DFA_Node *regEngine::DFA_Graph::addNode(regEngine::DFA_NodeType type) {
    if (type == DFA_DEAD) {
        return const_cast<regEngine::DFA_Node *>(getNode(0));
    }
    nodeCount++;
    auto[pt, success] = this->nodes.emplace(std::make_pair(nodeCount, std::make_unique<DFA_Node>(nodeCount,type)));
    if (type == DFA_START) {
        startNodeNumber = nodeCount;
    } else if (type == DFA_ACCEPT) {
        acceptNodeNumber.insert(nodeCount);
    }
    return pt->second.get();
}

regEngine::DFA_Edge *regEngine::DFA_Graph::addEdge(regEngine::DFA_Node *fromNode, regEngine::DFA_Node *toNode, char info) {
    if (fromNode != nullptr && toNode != nullptr) {
        auto[pt, success] = this->edges.emplace(std::make_unique<DFA_Edge>(fromNode, toNode, info));
        fromNode->edges.insert(std::make_pair(info, pt->get()));
        return pt->get();
    }
    return nullptr;
}



const regEngine::DFA_Node *regEngine::DFA_Graph::getNode(int number) const {
    return this->nodes.at(number).get();
}

const regEngine::DFA_Node *regEngine::DFA_Graph::getStartNode() const {
    return getNode(this->startNodeNumber);
}

std::unordered_set<const regEngine::DFA_Node *> regEngine::DFA_Graph::getAcceptNode() const {
    std::unordered_set<const regEngine::DFA_Node *> result;
    for (auto num : this->acceptNodeNumber) {
        result.insert(getNode(num));
    }
    return result;
}

const regEngine::DFA_Node *regEngine::DFA_Graph::getDeadNode() const {
    return getNode(this->deadNodeNumber);
}


std::string regEngine::DFA_Graph::toPrintable() const {
    std::stringstream printString;
    for (const auto &edge : edges) {
        if (edge->fromNode->number == startNodeNumber) {
            printString << "(start)  ";
        } else if (acceptNodeNumber.contains(edge->fromNode->number)) {
            printString << "(accept) ";
        } else if (edge->fromNode->number == deadNodeNumber) {
            printString << "(dead)   ";
        } else {
            printString << "(normal) ";
        }

        printString << edge->toPrintable();

        if (edge->toNode->number == startNodeNumber) {
            printString << "  (start)";
        } else if (acceptNodeNumber.contains(edge->toNode->number)) {
            printString << " (accept)";
        } else if (edge->toNode->number == deadNodeNumber) {
            printString << "   (dead)";
        } else {
            printString << " (normal)";
        }
        printString << "\n";
    }
    return printString.str();
}

std::set<int> regEngine::DFA_Graph::getAllNodeNumber() const {
    std::set<int> result;
    for (auto i = 0; i <= nodeCount; i++) {
        result.insert(i);
    }
    return result;
}

std::set<int> regEngine::DFA_Graph::getAcceptNodeNumber() const {
    std::set<int> result;
    result.insert(this->acceptNodeNumber.begin(), this->acceptNodeNumber.end());
    return result;
}
