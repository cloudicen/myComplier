#include "lexer.h"
#include <QtMath>

std::once_flag lexer::init;
QList<QPair<QString,tocken>> lexer::tockenList;
QSet<QString> lexer::keptWords={
    "PI","E","ORIGIN","SCALE","ROT","IS","FOR","FROM","TO","STEP","DRAW","DEF"
};

lexer::lexer()
{
    std::call_once(init,[&](){
        //这里要注意匹配优先级，例如匹配PI，应该放到匹配标识符之后，因为标识符可能以PI开头
        //至于保留字被占用的问题交给语义分析器，或者是在tocken的func中进行检查

        //SYMBOL
        //这里多了一步检查，如果
        tockenList.push_back(qMakePair<QString,tocken>("SYMBOL",tocken("_*.\\a.\\a*.\\d*",tockenType::SYMBOL,[=](tocken* _this,std::initializer_list<QVariant> list)->QVariant{
            QString name = list.begin()->toString();
            if(keptWords.contains(name))
            {
                foreach(auto item,tockenList)
                {
                    if(item.first == name)
                    {
                        qDebug() << "cast to kept word:" << name;
                        *_this = tocken(item.second);
                        break;
                    }
                }
            }
            else
            {
                _this->property["name"] = name;
            }
            return QVariant();
        },QJsonObject(),"symbol")));

        //const id
        tockenList.push_back(qMakePair<QString,tocken>("PI",tocken("P.I",tockenType::CONST_ID,QJsonObject({{"value",M_PI}}),"PI")));
        tockenList.push_back(qMakePair<QString,tocken>("E",tocken("E",tockenType::CONST_ID,QJsonObject({{"value",M_PI}}),"E")));

        //FUNC
        tockenList.push_back(qMakePair<QString,tocken>("SIN",tocken("S.I.N",tockenType::FUNC,[=](tocken* _this,std::initializer_list<QVariant> list)->QVariant{
            Q_UNUSED(_this);
            return QVariant(qSin(list.begin()->toDouble()));
        },QJsonObject(),"calculate sin")));
        tockenList.push_back(qMakePair<QString,tocken>("COS",tocken("C.O.S",tockenType::FUNC,[=](tocken* _this,std::initializer_list<QVariant> list)->QVariant{
            Q_UNUSED(_this);
            return QVariant(qCos(list.begin()->toDouble()));
        },QJsonObject(),"calculate cos")));
        tockenList.push_back(qMakePair<QString,tocken>("TAN",tocken("T.A.N",tockenType::FUNC,[=](tocken* _this,std::initializer_list<QVariant> list)->QVariant{
            Q_UNUSED(_this);
            return QVariant(qTan(list.begin()->toDouble()));
        },QJsonObject(),"calculate tan")));
        tockenList.push_back(qMakePair<QString,tocken>("LN",tocken("L.N",tockenType::FUNC,[=](tocken* _this,std::initializer_list<QVariant> list)->QVariant{
            Q_UNUSED(_this);
            return QVariant(qLn(list.begin()->toDouble()));
        },QJsonObject(),"calculate ln")));
        tockenList.push_back(qMakePair<QString,tocken>("EXP",tocken("E.X.P",tockenType::FUNC,[=](tocken* _this,std::initializer_list<QVariant> list)->QVariant{
            Q_UNUSED(_this);
            return QVariant(qExp(list.begin()->toDouble()));
        },QJsonObject(),"calculate exp")));
        tockenList.push_back(qMakePair<QString,tocken>("SQRT",tocken("S.Q.R.T",tockenType::FUNC,[=](tocken* _this,std::initializer_list<QVariant> list)->QVariant{
            Q_UNUSED(_this);
            return QVariant(qSqrt(list.begin()->toDouble()));
        },QJsonObject(),"calculate sqrt")));

        //keyword

        tockenList.push_back(qMakePair<QString,tocken>("ORIGIN",tocken("O.R.I.G.I.N",tockenType::KEYWORD,QJsonObject(),"set origin")));
        tockenList.push_back(qMakePair<QString,tocken>("SCALE",tocken("S.C.A.L.E",tockenType::KEYWORD,QJsonObject(),"set scale")));
        tockenList.push_back(qMakePair<QString,tocken>("ROT",tocken("R.O.T",tockenType::KEYWORD,QJsonObject(),"set rotation")));
        tockenList.push_back(qMakePair<QString,tocken>("IS",tocken("I.S",tockenType::KEYWORD,QJsonObject(),"assign value")));
        tockenList.push_back(qMakePair<QString,tocken>("FOR",tocken("F.O.R",tockenType::KEYWORD,QJsonObject(),"loop begin")));
        tockenList.push_back(qMakePair<QString,tocken>("FROM",tocken("F.R.O.M",tockenType::KEYWORD,QJsonObject(),"range begin")));
        tockenList.push_back(qMakePair<QString,tocken>("TO",tocken("T.O",tockenType::KEYWORD,QJsonObject(),"range end")));
        tockenList.push_back(qMakePair<QString,tocken>("STEP",tocken("S.T.E.P",tockenType::KEYWORD,QJsonObject(),"set step")));
        tockenList.push_back(qMakePair<QString,tocken>("DRAW",tocken("D.R.A.W",tockenType::KEYWORD,QJsonObject(),"start draw")));
        tockenList.push_back(qMakePair<QString,tocken>("DEF",tocken("D.E.F",tockenType::KEYWORD,QJsonObject(),"define a symbol")));

        //op
        tockenList.push_back(qMakePair<QString,tocken>("+",tocken("+",tockenType::OP,QJsonObject(),"plus")));
        tockenList.push_back(qMakePair<QString,tocken>("-",tocken("-",tockenType::OP,QJsonObject(),"minus")));
        tockenList.push_back(qMakePair<QString,tocken>("*",tocken("\\*",tockenType::OP,QJsonObject(),"multiply")));
        tockenList.push_back(qMakePair<QString,tocken>("/",tocken("/",tockenType::OP,QJsonObject(),"devide")));

        //mark
        tockenList.push_back(qMakePair<QString,tocken>("(",tocken("\\(",tockenType::MARK)));
        tockenList.push_back(qMakePair<QString,tocken>(")",tocken("\\)",tockenType::MARK)));
        tockenList.push_back(qMakePair<QString,tocken>(",",tocken(",",tockenType::MARK)));

        //number
        tockenList.push_back(qMakePair<QString,tocken>("NUMBER",tocken("\\d.\\d*.(\\e|(\\..\\d.\\d*))",tockenType::NUMBER,[=](tocken* _this,std::initializer_list<QVariant> list) -> QVariant{
            auto arg = *list.begin();
            {
                if(arg.canConvert(QMetaType::Double))
                {
                    _this->property["value"] = arg.toDouble();
                }
            }
            qInfo() << "val of this number: " << _this->property["value"];
            return QVariant(_this->property["value"]);},QJsonObject(),"number")));
    });
}

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
                int pos = tockenClass.second.match(str);
                if(pos != -1)
                {
                    auto to = tocken(tockenClass.second);
                    tockens.push_back(to);
                    to.act({str.mid(0,pos+1)});
                    qInfo() << to.toPrintable();
                    str=str.mid(pos+1,-1);
                    qDebug() << "after turncate" << str;
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
