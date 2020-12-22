#include <QCoreApplication>
#include "logger.h"
#include "lexer/tocken.h"
#include "lexer/scanner.h"
#include "lexer/lexer.h"
#include <functional>

int fn1();
bool fn2(int);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qInstallMessageHandler(outputMessage);
    QString testStr = "sin(12.34)sin(567.0)sin(88882222.05)FUCK!!!!!!";
    tocken TEST("(s.i.n.\\(.\\d*.(\\.|\\e).\\d*.\\))*");
    std::cout << testStr.toStdString() << '\n';
    std::cout << std::setw(TEST.match(testStr)) << " " << std::right << '^' << "~~~~~\n";
    tocken tocken2(TEST);
    std::cout << testStr.toStdString() << '\n';
    std::cout << std::setw(tocken2.match(testStr)) << " " << std::right << '^' << "~~~~~\n";
    QStringList sentence;
    QString str;
    QFile data("input.txt");
    if(data.open(QFile::ReadOnly))
    {
        QTextStream scanner(&data);
        while(!scanner.atEnd())
        {
            str = scanner.readLine();
            sentence << scanner::scan(str);
        }
        qInfo() << sentence;
        lexer().parseTocken(sentence);
    }
    return a.exec();
}
