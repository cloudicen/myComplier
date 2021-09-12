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

        const DFA_Graph& constructGraph();

        const DFA_Graph& minimizeGraph();

        int match(const std::string& str);

    private:
        void genTokenSet(const NFA_Graph& graph);

        void subSetMethod(const NFA_Graph& graph);

        void hopcroft();

        std::pair<std::set<int>,std::set<int>> spilt(const std::set<int>&);
    };
}


#endif //MY_COMPILER_DFA_H
