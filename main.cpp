#include <QCoreApplication>
#include "logger.h"
#include "lexer/tocken.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qInstallMessageHandler(outputMessage);
    QString testStr = "sin(12.34)sin(567.0)sin(88882222.05)FUCK!!!!!!";
    tocken TEST("(s.i.n.\\(.\\d*.(\\.|\\e).\\d*.\\))*");
    std::cout << testStr.toStdString() << '\n';
    std::cout << std::setw(TEST.match(testStr)) << " " << std::right << '^' << "~~~~~";
    return a.exec();
}
