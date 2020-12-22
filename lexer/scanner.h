#ifndef SCANNER_H
#define SCANNER_H

#include <QString>
#include <QTextStream>


class scanner
{
private:
    static QTextStream lineScan;
public:
    static QStringList scan(const QString &str);
};

#endif // SCANNER_H
