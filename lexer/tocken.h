#ifndef TOCKEN_H
#define TOCKEN_H

enum tockenType{
    CONST_ID,
    SYMBOL,
    FUNC,
    KEYWORD,
    OP,
    MARK,
    END,
    EMPTY,
    NUMBER
};

#include <QString>
#include "../regEngine/mdfa.h"

class tocken
{
public:
    tockenType type;
    int val;
    int* func;
    QString regExpr;
    mdfa* macher;
    tocken();
    void construct();

    bool match(const QString & str);
};

#endif // TOCKEN_H
