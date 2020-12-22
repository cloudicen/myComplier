#include "lexer.h"

QList<tocken> lexer::parseTocken(QStringList sentences)
{
    QList<tocken> tockens;
    foreach(auto str,sentences)
    {
        while(!str.isEmpty())
        {
            bool hasMatch=false;
            foreach(auto tockenClass,tockenList)
            {
                int pos = tockenClass.match(str);
                if(pos != -1)
                {
                    auto to = tocken(tockenClass);
                    to.info = str.mid(0,pos+1);
                    tockens.push_back(to);
                    to.act({str.mid(0,pos+1)});
                    qDebug() << tockenClass.toPrintable();
                    str=str.mid(pos+1,-1);
                    qDebug() << "turncate" << str;
                    hasMatch = true;
                    break;
                }
            }
            if(!hasMatch)
            {
                qInfo() << "sytax error";
                break;
            }
        }
    }
    return tockens;
}
