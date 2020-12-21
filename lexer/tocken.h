#ifndef TOCKEN_H
#define TOCKEN_H

#include "dfamatcher.h"

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

class tocken
{
private:
    QSharedPointer<QString> regExpr;
    QSharedPointer<dfaMatcher> matcher;
    tockenType type;
public:
    int val;
    int* func;
    tocken(const QString &_regExpr,tockenType _type=EMPTY,int _val=0,int* _func=nullptr):
        regExpr(QSharedPointer<QString>::create(_regExpr)),
        matcher(QSharedPointer<dfaMatcher>::create(*regExpr)),
        type(_type),
        val(_val),
        func(_func){};
    tocken(const tocken& other)
    {
        regExpr = other.regExpr;
        matcher = other.matcher;
        type = other.type;
        val = other.val;
        func = other.func;
    }
    int match(const QString & str);
};

#endif // TOCKEN_H
