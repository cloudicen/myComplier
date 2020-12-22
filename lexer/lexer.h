#ifndef LEXER_H
#define LEXER_H

#include <QList>
#include <QMap>
#include <mutex>
#include "tocken.h"



class lexer
{
private:
    static std::once_flag init;
    static QList<QPair<QString,tocken>> tockenList;
    static QSet<QString> keptWords;
public:
    lexer();
    QList<tocken>parseTocken(QStringList sentences);
};

#endif // LEXER_H
