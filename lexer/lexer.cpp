#include "lexer.h"
#include "mytockens.h"

std::once_flag lexer::init;
QSharedPointer<tockenDefine> lexer::defination;

lexer::lexer(QSharedPointer<tockenDefine> define)
{
    std::call_once(init,[&](){
        defination = define;
    });
}

QList<QSharedPointer<tocken>> lexer::parseTocken(QStringList sentences)
{
    QList<QSharedPointer<tocken>> tockens;
    foreach(auto str,sentences)
    {
        while(!str.isEmpty())
        {
            bool hasMatch=false;
            foreach(auto tockenClass,defination->tockenList)// 遍历tocken列表，按顺序匹配，其实就是一个NFA尝试回溯的过程
            {
                int pos = tockenClass.second->match(str);
                if(pos != -1)//如果找到了匹配
                {
                    auto currentString = str.mid(0,pos+1);//将当前的匹配字符串从输入中提取出来
                    auto matchTocken = QSharedPointer<tocken>::create(*tockenClass.second,currentString);//创建一个新的tocken实例
                    tockens.push_back(matchTocken);// 将tocken放入结果集中
                    matchTocken->act({currentString});
                    qDebug().noquote() << matchTocken->toPrintable();
                    str=str.mid(pos+1,-1);//将已经匹配的部分去除

                    qDebug() << "after turncate" << str;
                    hasMatch = true;
                    break;
                }
            }
            if(!hasMatch)//没有与任何一个定义的tocken类型匹配，说明有词法错误
            {
                qInfo() << "lexical error";
                break;
            }
        }
    }
    return tockens;
}
