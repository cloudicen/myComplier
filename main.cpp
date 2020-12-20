#include <QCoreApplication>
#include "util/treestructprinter.h"
#include "lexer/tocken.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << QChar('}').isLetter();
    regTree tree=regTree("w.h.i.l.e.\\(.\\d*.\\).{.\\w.\\w*.}");
    //regTree tree("(a|b)*.a.b.b");
    //regTree tree("a|b*");形如这样的前缀冲突模式目前无法工作，实际使用的时候应转换成多个符号，即拆分成多个dfa引擎分别匹配后再进行决定
    if(tree.analyze())
    {
       treeStructPrinter<regNode,2,64>::printTree(tree.getRootNode());

       auto test = new nfa(tree);
       test->parseNFA();
       test->print();
       test->testNFA("while(10){}");


       auto dfaTest = dfa(*test);
       dfaTest.parseDFA();
       dfaTest.toPrintable();
       dfaTest.testDFA("while(10){}");

       auto mdfaTest = mdfa(dfaTest);
       mdfaTest.parseMDFA();
       mdfaTest.print();
       mdfaTest.testMDFA("while(10){}");


    }
    else
    {
        qDebug() << "error occur";
    }
    return a.exec();
}
