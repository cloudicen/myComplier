﻿#include <QCoreApplication>
#include "logger.h"
#include "lexer/tocken.h"
#include "lexer/scanner.h"
#include "lexer/lexer.h"
#include "lexer/mytockens.h"
#include <functional>
#include "regEngine/unitTest.h"
#include "parser/parser.h"

int fn1();
bool fn2(int);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qInstallMessageHandler(outputMessage);
    /*
    QString testStr = "sin(1234)sin(5670)sin(8888222205)FUCK!!!!!!";
    tocken TEST("(s.i.n.\\(.\\d*.(\\.|\\e).\\d*.\\))*",tockenType::EMPTY);

    std::cout << testStr.toStdString() << '\n';
    std::cout << std::setw(TEST.match(testStr)) << " " << std::right << '^' << "~~~~~\n";
    tocken tocken2(TEST);
    std::cout << testStr.toStdString() << '\n';
    std::cout << std::setw(tocken2.match(testStr)) << " " << std::right << '^' << "~~~~~\n";
    */
    QStringList sentence;
    QString str;
    QFile data("input.txt");
    QList<QSharedPointer<tocken>> tockens;
    lexer lex(QSharedPointer<myTockens>(new myTockens));
    if(data.open(QFile::ReadOnly))
    {
        QTextStream scanner(&data);
        while(!scanner.atEnd())
        {
            str = scanner.readLine();
            sentence << scanner::scan(str);
        }
        tockens << lex.parseTocken(sentence);
    }
    parser p(tockens);
    p.analyze();
    return a.exec();
}
