#ifndef LEXER_H
#define LEXER_H

#include <QList>
#include <QMap>
#include <mutex>
#include "tockensDefine.h"
#include "tocken.h"


class lexer
{
private:
    static std::once_flag init;
public:
    static QSharedPointer<tockenDefine> defination;
public:
    lexer(QSharedPointer<tockenDefine> define);

    QList<QSharedPointer<tocken>>parseTocken(QStringList sentences);

    static QSharedPointer<tocken>getOriginTocken(const QString& TockeName)
    {
        return defination->getOriginTocken(TockeName);
    };
};

#endif // LEXER_H
