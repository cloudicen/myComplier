//
// Created by lk on 2021/9/11.
//

#ifndef MY_COMPILER_DFA_GRAPH_H
#define MY_COMPILER_DFA_GRAPH_H

#include "../base/Printable.h"
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <memory>
#include <sstream>

namespace regEngine {
    class DFA_Node;

    class DFA_Graph;

    enum DFA_NodeType {
        DFA_START,
        DFA_ACCEPT,
        DFA_DEAD,
        DFA_NORMAL
    };

    class DFA_Edge : public Printable {
    public:
        const char info;
    private:
        DFA_Node *fromNode;
        DFA_Node *toNode;

        friend DFA_Graph;
        friend DFA_Node;
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
        const DFA_NodeType nodeType;
    private:
        std::unordered_map<char,const DFA_Edge*> edges;

        friend DFA_Graph;
    public:
        explicit DFA_Node(int _number,DFA_NodeType _nodeType):number(_number),nodeType(_nodeType){};
        ~DFA_Node() = default;

        int getsMove(char ch) const{
            if (!edges.empty() && edges.contains(ch)){
                auto edge = edges.at(ch);
                return edge->toNode->number;
            } else {
                return 0;
            }
        }

        std::string toPrintable() const override;
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
        DFA_Graph() {this->nodes.emplace(std::make_pair(0, std::make_unique<DFA_Node>(0,DFA_DEAD)));}

        DFA_Graph(const DFA_Graph& other) = delete;

        DFA_Graph(DFA_Graph&& other)  noexcept {
            nodeCount = other.nodeCount;
            startNodeNumber = other.startNodeNumber;
            acceptNodeNumber = std::move(other.acceptNodeNumber);
            deadNodeNumber = other.deadNodeNumber;
            nodes = std::move(other.nodes);
            edges = std::move(other.edges);
        }

        DFA_Graph& operator=(DFA_Graph&& other) noexcept {
            nodeCount = other.nodeCount;
            startNodeNumber = other.startNodeNumber;
            acceptNodeNumber = std::move(other.acceptNodeNumber);
            deadNodeNumber = other.deadNodeNumber;
            nodes = std::move(other.nodes);
            edges = std::move(other.edges);
            return *this;
        }

        DFA_Node *addNode(DFA_NodeType type = DFA_NORMAL);

        DFA_Edge *addEdge(DFA_Node *fromNode, DFA_Node *toNode, char info);

        int getStartNodeNumber() const{return startNodeNumber;};

        int getDeadNodeNumber() const{return deadNodeNumber;};

        std::set<int> getAllNodeNumber() const;

        std::set<int> getAcceptNodeNumber() const;

        const DFA_Node *getNode(int number) const;

        const DFA_Node *getStartNode() const;

        std::unordered_set<const DFA_Node *> getAcceptNode() const;

        const DFA_Node *getDeadNode() const;

        std::string toPrintable() const override;
    };
}

#endif //MY_COMPILER_DFA_GRAPH_H
