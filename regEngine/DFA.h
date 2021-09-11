//
// Created by lk on 2021/9/12.
//

#ifndef MY_COMPILER_DFA_H
#define MY_COMPILER_DFA_H

#include "DFA_Graph.h"
#include "NFA.h"
#include "RegEngineDef.h"
#include <assert.h>


namespace regEngine{
    class DFA {
    private:
        std::shared_ptr<logger> LOGGER = LoggerFactory::getLogger("DFA");
        NFA nfa;
        DFA_Graph dfaGraph;
        std::unordered_set<char> tokenSet;
    public:
        explicit DFA(const std::string& regExpr):nfa(regExpr){};
        ~DFA() = default;

        void constructGraph();

        int match(const std::string& str);

    private:
        void genTokenSet(const NFA_Graph& graph);

        void subSetMethod(const NFA_Graph& graph);
    };
}


#endif //MY_COMPILER_DFA_H
