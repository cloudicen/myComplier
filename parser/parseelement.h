#ifndef PARSEELEMENT_H
#define PARSEELEMENT_H
#include <QStack>
#include "../lexer/lexer.h"
#include "../lexer/tocken.h"

class parseElement:public tocken
{
private:
    QString name="";
    bool terminal=false;
    QList<QPair<QSet<QSharedPointer<tocken>>,QStack<QSharedPointer<parseElement>>>> rules;
public:
    parseElement(QString _name,bool _terminate):name(_name),terminal(_terminate){
        property["string"] = name;
        property["node Parse from element"]=true;
    };
    parseElement(QSharedPointer<tocken> _tocken):tocken(*_tocken),name(property["string"].toString()),terminal(true)
    {
        QSet<QSharedPointer<tocken>> match;
        match.insert(_tocken);
        rules.append(qMakePair(match,QStack<QSharedPointer<parseElement>>()));
        property["node Parse from element"]=true;
    };

    bool isTerminal()
    {
        return terminal;
    }

    static QList<QSharedPointer<parseElement>> parseToElement(QList<QPair<QString,QSharedPointer<tocken>>> terminals)
    {
        QList<QSharedPointer<parseElement>> result;
        foreach(auto tocken,terminals)
        {
            result.push_back(QSharedPointer<parseElement>::create(tocken.second));
        }
        return result;
    }

    QSharedPointer<parseElement> addRule(std::initializer_list<QSharedPointer<tocken>> matchTockens,std::initializer_list<QSharedPointer<tocken>> nextStack)
    {
        QStack<QSharedPointer<parseElement>> stack;
        foreach(auto tock,nextStack)
        {
            assert(tock);
            if(tock.dynamicCast<parseElement>() == nullptr)
            {
                qDebug() << "cast tocken to element" << tock->property["string"] << tock->property["type"];
                stack.push_front(QSharedPointer<parseElement>::create(tock));
            }
            else
            {
                qDebug() << "no need to cast" << tock->property["string"];
                stack.push_front(tock.staticCast<parseElement>());
            }
        }
        QSet<QSharedPointer<tocken>> ruleSet;
        foreach(auto tock,matchTockens)
        {
            qDebug() << "add rule key" << tock->property["string"];
            qDebug() << (*tock==*lexer::getOriginTocken(";"));
            ruleSet.insert(tock);
        }
            qDebug() << "";
        rules.append(qMakePair(ruleSet,stack));
        return sharedFromThis().staticCast<parseElement>();
    }

    QPair<bool,QStack<QSharedPointer<parseElement>>> spand(QSharedPointer<tocken> t)
    {
        foreach(auto element,rules)
        {
            auto tockens = element.first;
            foreach(auto tocken,tockens)
            {
                if(*tocken == *t)
                {
                    qDebug() << "match" <<  tocken->property["string"] << t->property["string"];
                    qDebug() << "";
                    return  qMakePair(true,element.second);
                }
                else
                {
                    qDebug() << "miss match" << tocken->property["string"] << t->property["string"];
                }
            }
        }
        qDebug() << "";
        return qMakePair(false,QStack<QSharedPointer<parseElement>>());
    }
};

#endif // PARSEELEMENT_H
