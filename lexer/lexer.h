#ifndef LEXER_H
#define LEXER_H

#include <QList>
#include "tocken.h"



class lexer
{
private:
    QList<tocken> tockenList;
public:
    lexer(){
        //QList<tocken> lexer::tockenList={
        tockenList.push_back(tocken("w.h.i.l.e",tockenType::KEYWORD,[=](tocken* _this,std::initializer_list<QVariant> list) -> QVariant{
            auto temp = _this->val.toInt();
            foreach(auto arg,list)
            {
                qInfo() << "args: " << arg;
                temp++;

            }
            _this->val = temp;
            qInfo() << "val: " << _this->val;
            return QVariant(_this->val);},0,"while loop"));
            tockenList.push_back(tocken("_*.\\a.\\a*.\\d*",tockenType::SYMBOL));
            tockenList.push_back(tocken("\\(",tockenType::MARK));
            tockenList.push_back(tocken("\\)",tockenType::MARK));
            tockenList.push_back(tocken("\\d.\\d*.(\\e|((\\e|\\.).\\d.\\d*))",tockenType::NUMBER));
        //};
    }
    QList<tocken>parseTocken(QStringList sentences);
};

#endif // LEXER_H
