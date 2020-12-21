#include "mdfa.h"
#include "../util/treestructprinter.h"
namespace regEngine
{

void test()
{
    regTree idtree=regTree("(_|\\a).\\w*.\\d*");
    regTree tree=regTree("(s.i.n.\\(.\\d*.(\\.|\\e).\\d*.\\))*");
    QString testStr = "sin(12.34)sin(567.0)..";
    //regTree tree("(a|b)*.a.b.b");
    //regTree tree("a|a.b.b|a*.b.b*");//形如这样的前缀冲突模式,其匹配规则就是从前向后依次匹配，前缀冲突的项就以排在前面的为准
    //如果表达式是[a.b.b|a*.b.b*]其无法匹配aabbb
    //反之，把顺序颠倒，变成[a*.b.b*|a.b.b],就能够正确匹配
    //所以一般是拆分成单独的状态机，按照自定的语法规则和优先级逐个尝试匹配
    if(tree.analyze())
    {
       treeStructPrinter<regNode,2,64>::printTree(tree.getRootNode());

       auto test = nfa(tree);
       test.parseNFA();
       test.print();
       test.testNFA(testStr);

       auto dfaTest = dfa(test);
       dfaTest.parseDFA();
       dfaTest.toPrintable();
       dfaTest.testDFA(testStr);

       auto mdfaTest = mdfa(dfaTest);
       mdfaTest.parseMDFA();
       mdfaTest.print();
       mdfaTest.testMDFA(testStr);

       std::cout << testStr.toStdString() << '\n';
       std::cout << std::setw(mdfaTest.match("sin(12.34)sin(567.0)..")+1) << std::right << '^' << "~~~~~";
    }
    else
    {
        qDebug() << "error occur";
    }
}

}


