//
// Created by lk on 2021/9/11.
//

#ifndef MY_COMPILER_DFA_GRAPH_H
#define MY_COMPILER_DFA_GRAPH_H

#include "../base/Printable.h"
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <sstream>

namespace regEngine {
    class DFA_Node;

    class DFA_Graph;

    class DFA_Edge : public Printable {
    public:
        const char info;
    private:
        DFA_Node *fromNode;
        DFA_Node *toNode;

        friend DFA_Graph;
    public:
        explicit DFA_Edge(DFA_Node *_fromNode, DFA_Node *_toNode, char _info) : fromNode(_fromNode), toNode(_toNode), info(_info) {};

        ~DFA_Edge() = default;

        bool accept(char ch) const {
            return ch == info;
        }

        std::string toPrintable() const override;
    };

    class DFA_Node : public Printable {
    public:
        const int number;
    private:
        std::unordered_multimap<char,const DFA_Edge*> edges;

        friend DFA_Graph;
    public:
        explicit DFA_Node(int _number):number(_number){};
        ~DFA_Node() = default;

        std::string toPrintable() const override;
    };

    enum DFA_NodeType {
        DFA_START,
        DFA_ACCEPT,
        DFA_DEAD,
        DFA_NORMAL
    };

    class DFA_Graph : public Printable{
    private:
        int nodeCount = 0;
        int startNodeNumber = 0;
        std::unordered_set<int> acceptNodeNumber;
        int deadNodeNumber = 0;
        std::unordered_map<int,std::unique_ptr<DFA_Node>> nodes;
        std::unordered_set<std::unique_ptr<DFA_Edge>> edges;
    public:
        DFA_Graph() {this->nodes.emplace(std::make_pair(0, std::make_unique<DFA_Node>(0)));}
        DFA_Node *addNode(DFA_NodeType type = DFA_NORMAL);

        DFA_Edge *addEdge(DFA_Node *fromNode, DFA_Node *toNode, char info);

        const DFA_Node *getNode(int number) const;

        const DFA_Node *getStartNode() const;

        std::unordered_set<const DFA_Node *> getAcceptNode() const;

        const DFA_Node *getDeadNode() const;

        std::string toPrintable() const override;
    };
}

#endif //MY_COMPILER_DFA_GRAPH_H
