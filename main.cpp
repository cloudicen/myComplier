#include "regEngine/NFA.h"
#include "regEngine/RegTree.h"
#include "regEngine/DFA.h"
#include "regEngine/NFA_Graph.h"

int main() {
    std::string regExpr = "(\\w|b)*abb";
    std::string matchString = "._1223341""abacabb";
//    regEngine::RegTree tree(regExpr);
//    tree.generateRegTree();
//    regEngine::NFA_Graph graph_nfa;
//    auto a = graph_nfa.addNode(true);
//    auto b = graph_nfa.addNode();
//    auto c = graph_nfa.addNode(false, true);
//    graph_nfa.addEdge(a,b,"1");
//    graph_nfa.addEdge(a,c,"2");
//    graph_nfa.addEdge(b,c,"3");
//    printf("%s", graph_nfa.toPrintable().c_str());
//    regEngine::NFA nfa(regExpr);
//    nfa.constructGraph();
//    nfa.match(matchString);
    regEngine::DFA dfa(regExpr);
    dfa.constructGraph();
    dfa.minimizeGraph();
    dfa.match(matchString);
    return 0;
}
