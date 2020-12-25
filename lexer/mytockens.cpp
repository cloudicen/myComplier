#include "mytockens.h"

void myTockens::loadDefination()
{
    //这里要注意匹配优先级，例如匹配PI，应该放到匹配标识符之后，因为标识符可能以PI开头
    //至于保留字被占用的问题交给语义分析器，或者是在tocken的func中进行检查
    //这里多了一步检查，如果匹配的SYMBOL是保留字，则转换tocken的类型到该保留字上

    //SYMBOL
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("SYMBOL",QSharedPointer<tocken>::create("_*.\\a.\\a*.\\d*",tockenType::SYMBOL,[&](QSharedPointer<tocken> _this,std::initializer_list<QVariant> list)->QVariant{
        static QSet<QString> keptWords={
            "PI","E",
            "SIN","COS","TAN","LN","EXP","SQRT",
            "ORIGIN","SCALE","ROT","IS","FOR","FROM","TO","STEP","DRAW","DEF"
        };
        QString name = list.begin()->toString();
        if(keptWords.contains(name))
        {
            foreach(auto item,tockenList)
            {
                if(item.first == name)
                {
                    qDebug() << "need to cast to kept word:" << name;
                    //调用了复制构造函数，这里因为传入的是this指针，所以没法修改自身指向已有的对象
                    *_this = *item.second;
                    break;
                }
            }
        }
        else
        {
            _this->property["name"] = name;
        }
        return QVariant();
    },QJsonObject(),"","a symbol to represent value")));

    //const id
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("PI",QSharedPointer<tocken>::create("P.I",tockenType::CONST_ID,QJsonObject({{"value",M_PI}}),"PI","Ratio of circumference to diameter")));
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("E",QSharedPointer<tocken>::create("E",tockenType::CONST_ID,QJsonObject({{"value",M_E}}),"E","e")));

    //FUNC
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("SIN",QSharedPointer<tocken>::create("S.I.N",tockenType::FUNC,[=](QSharedPointer<tocken> _this,std::initializer_list<QVariant> list)->QVariant{
        Q_UNUSED(_this);
        return QVariant(qSin(list.begin()->toDouble()));
    },QJsonObject(),"SIN","calculate sin")));
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("COS",QSharedPointer<tocken>::create("C.O.S",tockenType::FUNC,[=](QSharedPointer<tocken> _this,std::initializer_list<QVariant> list)->QVariant{
        Q_UNUSED(_this);
        return QVariant(qCos(list.begin()->toDouble()));
    },QJsonObject(),"COS","calculate cos")));
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("TAN",QSharedPointer<tocken>::create("T.A.N",tockenType::FUNC,[=](QSharedPointer<tocken> _this,std::initializer_list<QVariant> list)->QVariant{
        Q_UNUSED(_this);
        return QVariant(qTan(list.begin()->toDouble()));
    },QJsonObject(),"TAN","calculate tan")));
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("LN",QSharedPointer<tocken>::create("L.N",tockenType::FUNC,[=](QSharedPointer<tocken> _this,std::initializer_list<QVariant> list)->QVariant{
        Q_UNUSED(_this);
        return QVariant(qLn(list.begin()->toDouble()));
    },QJsonObject(),"LN","calculate ln")));
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("EXP",QSharedPointer<tocken>::create("E.X.P",tockenType::FUNC,[=](QSharedPointer<tocken> _this,std::initializer_list<QVariant> list)->QVariant{
        Q_UNUSED(_this);
        return QVariant(qExp(list.begin()->toDouble()));
    },QJsonObject(),"EXP","calculate exp")));
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("SQRT",QSharedPointer<tocken>::create("S.Q.R.T",tockenType::FUNC,[=](QSharedPointer<tocken> _this,std::initializer_list<QVariant> list)->QVariant{
        Q_UNUSED(_this);
        return QVariant(qSqrt(list.begin()->toDouble()));
    },QJsonObject(),"SQRT","calculate sqrt")));

    //keyword

    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("ORIGIN",QSharedPointer<tocken>::create("O.R.I.G.I.N",tockenType::KEYWORD,QJsonObject(),"ORIGIN","set origin")));
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("SCALE",QSharedPointer<tocken>::create("S.C.A.L.E",tockenType::KEYWORD,QJsonObject(),"SCALE","set scale")));
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("ROT",QSharedPointer<tocken>::create("R.O.T",tockenType::KEYWORD,QJsonObject(),"ROT","set rotation")));
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("IS",QSharedPointer<tocken>::create("I.S",tockenType::KEYWORD,QJsonObject(),"IS","assign value")));
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("FOR",QSharedPointer<tocken>::create("F.O.R",tockenType::KEYWORD,QJsonObject(),"FOR","loop begin")));
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("FROM",QSharedPointer<tocken>::create("F.R.O.M",tockenType::KEYWORD,QJsonObject(),"FROM","range begin")));
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("TO",QSharedPointer<tocken>::create("T.O",tockenType::KEYWORD,QJsonObject(),"TO","range end")));
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("STEP",QSharedPointer<tocken>::create("S.T.E.P",tockenType::KEYWORD,QJsonObject(),"STEP","set step")));
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("DRAW",QSharedPointer<tocken>::create("D.R.A.W",tockenType::KEYWORD,QJsonObject(),"DRAW","start draw")));
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("DEF",QSharedPointer<tocken>::create("D.E.F",tockenType::KEYWORD,QJsonObject(),"DEF","define a symbol")));

    //op
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("+",QSharedPointer<tocken>::create("+",tockenType::OP,QJsonObject(),"+","plus")));
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("-",QSharedPointer<tocken>::create("-",tockenType::OP,QJsonObject(),"-","minus")));
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("*",QSharedPointer<tocken>::create("\\*",tockenType::OP,QJsonObject(),"*","multiply")));
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("/",QSharedPointer<tocken>::create("/",tockenType::OP,QJsonObject(),"/","devide")));
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("=",QSharedPointer<tocken>::create("=",tockenType::OP,QJsonObject(),"=","equal")));

    //mark
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("(",QSharedPointer<tocken>::create("\\(",tockenType::MARK,QJsonObject(),"(")));
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>(")",QSharedPointer<tocken>::create("\\)",tockenType::MARK,QJsonObject(),")")));
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>(",",QSharedPointer<tocken>::create(",",tockenType::MARK,QJsonObject(),",")));

    //number
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("NUMBER",QSharedPointer<tocken>::create("\\d.\\d*.(\\e|(\\..\\d.\\d*))",tockenType::NUMBER,[=](QSharedPointer<tocken> _this,std::initializer_list<QVariant> list) -> QVariant{
        auto arg = *list.begin();
        {
            if(arg.canConvert(QMetaType::Double))
            {
                _this->property["value"] = arg.toDouble();
            }
        }
        qDebug() << "val of this number: " << _this->property["value"];
        return QVariant(_this->property["value"]);},QJsonObject(),"","a number val")));

    //END
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>(";",QSharedPointer<tocken>::create(";",tockenType::END,QJsonObject(),";","end of a sentence.")));

    //empty
    tockenList.push_back(qMakePair<QString,QSharedPointer<tocken>>("EMPTY",QSharedPointer<tocken>::create()));
}
