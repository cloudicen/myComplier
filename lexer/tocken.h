#ifndef TOCKEN_H
#define TOCKEN_H

#include "dfamatcher.h"
#include "QVariant"

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
    QSharedPointer<std::function<QVariant(tocken*,std::initializer_list<QVariant>)>> func;
    tockenType type;
public:
    QVariant val;
    QString info;
    tocken(const QString &_regExpr,tockenType _type=EMPTY,std::function<QVariant(tocken*,std::initializer_list<QVariant>)> _func=nullptr,QVariant _val=0,const QString & _info=""):
        regExpr(QSharedPointer<QString>::create(_regExpr)),
        matcher(QSharedPointer<dfaMatcher>::create(*regExpr)),
        func(QSharedPointer<std::function<QVariant(tocken*,std::initializer_list<QVariant>)>>::create(_func)),
        type(_type),
        val(_val),
        info(_info){};
    tocken(const tocken& other)
    {
        regExpr = other.regExpr;
        matcher = other.matcher;
        type = other.type;
        val = other.val;
        func = other.func;
    }

    QVariant act(std::initializer_list<QVariant> args)
    {
        QVariant ret;
        if(!func.isNull() && (*func))
        {
            ret = (*func)(this,args);
        }
        return ret;
    }

    int match(const QString & str);

    QString toPrintable()
    {
        QString str="type:%1 info:%2";
        switch (type)
        {
        case FUNC:
            str = str.arg("FUNC");
            break;
        case KEYWORD:
            str = str.arg("KEYWORD");
            break;
        case OP:
            str = str.arg("OP");
            break;
        case MARK:
            str = str.arg("MARK");
            break;
        case END:
            str = str.arg("END");
            break;
        case EMPTY:
            str = str.arg("EMPTY");
            break;
        case NUMBER:
            str = str.arg("NUMBER");
            break;
        case CONST_ID:
            str = str.arg("CONST_ID");
            break;
        case SYMBOL:
            str = str.arg("SYMBOL");

        }
        return str.arg(info);
    }
};

#endif // TOCKEN_H
