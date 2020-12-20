#include "tocken.h"


tocken::tocken()
{
    ;
}

void tocken::construct()
{
    auto reg = regTree(regExpr);
    reg.analyze();
    auto nfaGraph = nfa(reg);
    nfaGraph.parseNFA();
    auto dfaGraph = dfa(nfaGraph);
    dfaGraph.parseDFA();
    macher = new mdfa(dfaGraph);
    macher->parseMDFA();
}

bool tocken::match(const QString & str)
{
    return macher->match(str);
}
