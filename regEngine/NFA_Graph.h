//
// Created by lk on 2021/9/10.
//

#ifndef MY_COMPILER_NFA_GRAPH_H
#define MY_COMPILER_NFA_GRAPH_H

#include "../base/Printable.h"
#include <memory>
#include "RegEngineDef.h"
#include <sstream>
#include <stack>
#include <unordered_map>
#include <unordered_set>

namespace regEngine {

    class nfaNode;

    class NFA_Graph;

    class nfaEdge : public Printable {
    public:
        static std::string EPS;
    private:
        const nfaNode *fromNode;
        const nfaNode *toNode;
        std::string info;

        /**
         * friend classes
         */
        friend class nfaNode;

        friend class NFA_Graph;

    public:
        explicit nfaEdge(const nfaNode *_from, const nfaNode *_to, std::string _info = nfaEdge::EPS)
                : fromNode(_from), toNode(_to), info(std::move(_info)) {};

        bool accept(char ch) const;

        std::string toPrintable() const override;
    };

    class nfaNode : public Printable {

    private:
        int number;
        std::unordered_set<const nfaEdge *> epsEdges;
        std::unordered_multimap<std::string, const nfaEdge *> edges;

        /**
         * friend classes
         */
        friend class NFA_Graph;

    public:
        explicit nfaNode(int _number = -1) : number(_number) {};

        std::unordered_set<const nfaNode *> getEpsMove() const;

        std::unordered_set<const nfaNode *> getsMove(char ch) const;

        std::string toPrintable() const override;
    };

    enum NFA_NodeType {
        NFA_START,
        NFA_ACCEPT,
        NFA_NORMAL
    };

    class NFA_Graph : public Printable {
    private:
        int nodeCount = 0;
        int startNodeNumber = 0;
        int acceptNodeNumber = 0;
        std::unordered_multimap<std::string, std::unique_ptr<nfaEdge>> edges;
        std::unordered_map<int, std::unique_ptr<nfaNode>> nodes;

    public:
        nfaNode *addNode(NFA_NodeType type = NFA_NORMAL);

        nfaEdge *addEdge(nfaNode *fromNode, nfaNode *toNode, const std::string &info = nfaEdge::EPS);

        const nfaNode *getNode(int number) const;

        const nfaNode *getStartNode() const;

        const nfaNode *getAcceptNode() const;

        std::unordered_set<const nfaNode *> getEpsClosure(std::unordered_set<const nfaNode *> &&curState) const;

        std::unordered_set<const nfaNode *> getsMove(std::unordered_set<const nfaNode *> &&curState, char curMatchChar) const;

        std::string toPrintable() const override;
    };

} // namespace regEngine

#endif // MY_COMPILER_NFA_GRAPH_H
