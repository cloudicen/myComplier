#include <QCoreApplication>
#include "util/treestructprinter.h"
#include "lexer/tocken.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    regTree* tree= new regTree("T*");
    //regTree tree("w.h.i.l.e.\\(.\\d*.\\).{.\\s*.}");
    //regTree tree("a|b*");形如这样的前缀冲突模式目前无法工作，实际使用的时候应转换成多个符号，即拆分成多个dfa引擎分别匹配后再进行决定
    if(tree->analyze())
    {
       treeStructPrinter<regNode,2,64>::printTree(tree->getRootNode());

       auto test = new nfa(*tree);
       test->parseNFA();
       test->toPrintable();
       test->testNFA("while(123){}");

       delete tree;

       auto dfaTest = dfa(*test);
       dfaTest.parseDFA();
       dfaTest.toPrintable();
       dfaTest.testDFA("while(123){}");

       delete test;

       dfaTest.testDFA("while(123){}");

       auto mdfaTest = mdfa(dfaTest);
       mdfaTest.parseMDFA();
       mdfaTest.toPrintable();
       mdfaTest.testMDFA("99");

    }
    else
    {
        qDebug() << "error occur";
    }


    auto tock = tocken();
    tock.regExpr = "T*";
    tock.construct();
    qDebug() << (tock.match("99") ? "valid" : "invalid");
    return a.exec();
}
