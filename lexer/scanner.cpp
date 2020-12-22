#include "scanner.h"
QTextStream scanner::lineScan;

QStringList scanner::scan(const QString &str)
{
    QString lineStr=str;
    QStringList sentence;
    int idx1 = lineStr.indexOf("--");
    int idx2 = lineStr.indexOf("//");
    if(idx1 != -1 && idx2 != -1)
    {
        lineStr.truncate(idx1 < idx2 ? idx1:idx2);
    }
    else if(idx1!=-1)
    {
        lineStr.truncate(idx1);
    }
    else if(idx2 != -1)
    {
        lineStr.truncate(idx2);
    }
    lineScan.setString(&lineStr);
    while(!lineScan.atEnd())
    {
        QString sub;
        lineScan >> sub;
        sentence << sub.split(';',Qt::SkipEmptyParts);
    }
    return sentence;
}
