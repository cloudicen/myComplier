#ifndef TOCKENSDEFINE_H
#define TOCKENSDEFINE_H
#include "tocken.h"
#include "QtMath"

class tockenDefine
{
public:
    QList<QPair<QString,QSharedPointer<tocken>>> tockenList;

    tockenDefine(){loadDefination();};

    virtual void loadDefination(){};

    QSharedPointer<tocken> getOriginTocken(const QString & tockenName)
    {
        foreach(auto item,tockenList)
        {
            if(item.first == tockenName)
            {
                return item.second;
            }
        }
        return nullptr;
    };
};

#endif // TOCKENSDEFINE_H

