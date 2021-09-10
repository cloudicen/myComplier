//
// Created by lk on 2021/9/10.
//

#ifndef MY_COMPILER_NFA_H
#define MY_COMPILER_NFA_H

#include "NFA_Graph.h"
#include "RegTree.h"

namespace regEngine {
    class NFA {
    private:
        std::shared_ptr<logger> LOGGER = LoggerFactory::getLogger("NFA");
        RegTree regTree;
        NFA_Graph nfaGraph;
    public:
        NFA(const std::string& regExpr):regTree(regExpr){};
        ~NFA() = default;

        void constructGraph();

    private:
        void Thompson(RegNode* subTree);
    };
}

#endif //MY_COMPILER_NFA_H
