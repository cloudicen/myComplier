﻿#ifndef TOCKEN_H
#define TOCKEN_H

#include "dfamatcher.h"
#include "QJsonDocument"
#include "QJsonObject"
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
    void castType();
public:
    tockenType type;
    QJsonObject property;
    tocken()=default;
    tocken(const QString &_regExpr,tockenType _type,std::function<QVariant(tocken*,std::initializer_list<QVariant>)> _func,QJsonObject _property=QJsonObject(),const QString & _info=""):
        regExpr(QSharedPointer<QString>::create(_regExpr)),
        matcher(QSharedPointer<dfaMatcher>::create(*regExpr)),
        func(QSharedPointer<std::function<QVariant(tocken*,std::initializer_list<QVariant>)>>::create(_func)),
        type(_type),
        property(_property){
        property["discription"] = _info;
        castType();
    };

    tocken(const QString &_regExpr,tockenType _type,QJsonObject _property=QJsonObject(),const QString & _info=""):
        regExpr(QSharedPointer<QString>::create(_regExpr)),
        matcher(QSharedPointer<dfaMatcher>::create(*regExpr)),
        func(QSharedPointer<std::function<QVariant(tocken*,std::initializer_list<QVariant>)>>::create(nullptr)),
        type(_type),
        property(_property){
        property["discription"] = _info;
        castType();
    };

    tocken(const tocken& other)
    {
        regExpr = other.regExpr;
        matcher = other.matcher;
        property = other.property;
        type = other.type;
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
        QString str="properties:\n";
        return str.append(QJsonDocument(property).toJson());
    }
};

#endif // TOCKEN_H
