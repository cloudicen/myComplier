#include "parser.h"

void parser::getNextTocken()
{
    if(index+1 < tockens.length())
    {
        index++;
        currentTocken=tockens.at(index);
    }
    else
    {
        currentTocken=lexer::getOriginTocken("EMPTY");
    }
    qInfo().noquote() << "get next tocken" << currentTocken->toPrintable();
}

void parser::putTockenback()
{
    if(index-1 >= 0)
    {
        index--;
        currentTocken=tockens.at(index);
    }
    else
    {
        currentTocken=lexer::getOriginTocken("EMPTY");
    }
}


bool parser::analyze()
{
    while(!matchStack.isEmpty())
    {
        qInfo().noquote() << "curren tocken" << currentTocken->toPrintable();
        qInfo().noquote() << "curren match stack top"<< matchStack.top()->toPrintable();
        auto result =  matchStack.top().staticCast<parseElement>()->spand(currentTocken);//查表
        if(result.first == true)//成功找到规则
        {
            //如果是当前的符号是终结符则读入下一个
            if(result.second.isEmpty() && matchStack.top().staticCast<parseElement>()->isTerminal())
            {
                getNextTocken();
            }
            qInfo().noquote() << "pop:" << matchStack.top().staticCast<parseElement>()->toPrintable();
            matchStack.pop();//弹栈
            foreach(auto pt,result.second)//将返回的展开符号逐个压栈
            {
                qInfo().noquote() << "push:" << pt.staticCast<parseElement>()->toPrintable();
                matchStack.push(pt);
            }
        }
        else
        {
            qInfo() << "syntax error";
            return false;
        }
    }
    qInfo() << "syntax analyze done with no error.";
    return true;
}

parser::parser(QList<QSharedPointer<tocken>> _tockens):tockens(_tockens)
{
    //定义非终结符
    auto sentence=QSharedPointer<parseElement>(new parseElement("sentence",false));
    auto command=QSharedPointer<parseElement>(new parseElement("command",false));
    auto setOrigin=QSharedPointer<parseElement>(new parseElement("setOrigin",false));
    auto setRotation=QSharedPointer<parseElement>(new parseElement("setRotation",false));
    auto forDraw=QSharedPointer<parseElement>(new parseElement("forDraw",false));
    auto defineSymbol=QSharedPointer<parseElement>(new parseElement("defineSymbol",false));
    auto assignValue=QSharedPointer<parseElement>(new parseElement("assignValue",false));
    auto initAssin=QSharedPointer<parseElement>(new parseElement("initAssin",false));
    auto assign=QSharedPointer<parseElement>(new parseElement("assignValue",false));
    auto val=QSharedPointer<parseElement>(new parseElement("val",false));
    auto coordinate=QSharedPointer<parseElement>(new parseElement("coordinate",false));
    auto singleValue=QSharedPointer<parseElement>(new parseElement("singleValue",false));
    auto func=QSharedPointer<parseElement>(new parseElement("func",false));
    auto expr=QSharedPointer<parseElement>(new parseElement("expr",false));
    auto empty=QSharedPointer<parseElement>(new parseElement("empty",false));

    //转换终结符
    auto terminals = parseElement::parseToElement(lexer::defination->tockenList);

    ruleTable=QVector<QSharedPointer<tocken>>({sentence,command,
                                               setOrigin,setRotation,forDraw,defineSymbol,assignValue,
                                               val,coordinate,singleValue,expr,func});

    foreach(auto ele,terminals)
    {
        ruleTable.push_back(ele);
    }
    empty->addRule({lexer::getOriginTocken("EMPTY")},{lexer::getOriginTocken("EMPTY")});

    //添加表驱动规则
    sentence->addRule({lexer::getOriginTocken("EMPTY")},{})
            ->addRule({lexer::getOriginTocken("ORIGIN"),
                       lexer::getOriginTocken("FOR"),
                       lexer::getOriginTocken("ROT"),
                       lexer::getOriginTocken("DEF"),
                       lexer::getOriginTocken("SYMBOL")},{command,lexer::getOriginTocken(";"),sentence});

    command->addRule({lexer::getOriginTocken("ORIGIN")},{setOrigin})
            ->addRule({lexer::getOriginTocken("ROT")},{setRotation})
            ->addRule({lexer::getOriginTocken("FOR")},{forDraw})
            ->addRule({lexer::getOriginTocken("DEF")},{defineSymbol})
            ->addRule({lexer::getOriginTocken("SYMBOL")},{assignValue});

    setOrigin->addRule({lexer::getOriginTocken("ORIGIN")},{lexer::getOriginTocken("ORIGIN"),lexer::getOriginTocken("IS"),coordinate});

    setRotation->addRule({lexer::getOriginTocken("ROT")},{lexer::getOriginTocken("ROT"),lexer::getOriginTocken("IS"),singleValue});

    forDraw->addRule({lexer::getOriginTocken("FOR")},{lexer::getOriginTocken("FOR"),lexer::getOriginTocken("SYMBOL"),lexer::getOriginTocken("FROM"),singleValue,lexer::getOriginTocken("TO"),singleValue,lexer::getOriginTocken("STEP"),singleValue,lexer::getOriginTocken("DRAW"),coordinate});

    defineSymbol->addRule({lexer::getOriginTocken("DEF")},{lexer::getOriginTocken("DEF"),lexer::getOriginTocken("SYMBOL"),initAssin});

    assignValue->addRule({lexer::getOriginTocken("SYMBOL")},{lexer::getOriginTocken("SYMBOL"),assign});

    initAssin->addRule({lexer::getOriginTocken("IS")},{lexer::getOriginTocken("IS"),val})
            ->addRule({lexer::getOriginTocken("=")},{lexer::getOriginTocken("="),val})
            ->addRule({lexer::getOriginTocken(";")},{});

    assign->addRule({lexer::getOriginTocken("IS")},{lexer::getOriginTocken("IS"),val})
            ->addRule({lexer::getOriginTocken("=")},{lexer::getOriginTocken("="),val});

    val->addRule({lexer::getOriginTocken("(")},{coordinate})
            ->addRule({lexer::getOriginTocken("PI"),
                       lexer::getOriginTocken("E"),
                       lexer::getOriginTocken("SYMBOL"),
                       lexer::getOriginTocken("NUMBER"),
                       lexer::getOriginTocken("SIN"),
                       lexer::getOriginTocken("COS"),
                       lexer::getOriginTocken("TAN"),
                       lexer::getOriginTocken("LN"),
                       lexer::getOriginTocken("EXP"),
                       lexer::getOriginTocken("SQRT")},{singleValue});

    coordinate->addRule({lexer::getOriginTocken("(")},{lexer::getOriginTocken("("),singleValue,lexer::getOriginTocken(","),singleValue,lexer::getOriginTocken(")")});

    singleValue->addRule({lexer::getOriginTocken("SIN"),
                          lexer::getOriginTocken("COS"),
                          lexer::getOriginTocken("TAN"),
                          lexer::getOriginTocken("LN"),
                          lexer::getOriginTocken("EXP"),
                          lexer::getOriginTocken("SQRT")},{func,lexer::getOriginTocken("("),val,lexer::getOriginTocken(")"),expr})
            ->addRule({lexer::getOriginTocken("PI")},{lexer::getOriginTocken("PI"),expr})
            ->addRule({lexer::getOriginTocken("E")},{lexer::getOriginTocken("E"),expr})
            ->addRule({lexer::getOriginTocken("NUMBER")},{lexer::getOriginTocken("NUMBER"),expr})
            ->addRule({lexer::getOriginTocken("SYMBOL")},{lexer::getOriginTocken("SYMBOL"),expr})
            ->addRule({lexer::getOriginTocken("(")},{lexer::getOriginTocken("("),singleValue,lexer::getOriginTocken(")"),expr});

    expr->addRule({lexer::getOriginTocken("+")},{lexer::getOriginTocken("+"),singleValue})
            ->addRule({lexer::getOriginTocken("-")},{lexer::getOriginTocken("-"),singleValue})
            ->addRule({lexer::getOriginTocken("*")},{lexer::getOriginTocken("*"),singleValue})
            ->addRule({lexer::getOriginTocken("/")},{lexer::getOriginTocken("/"),singleValue})
            ->addRule({lexer::getOriginTocken("=")},{lexer::getOriginTocken("="),lexer::getOriginTocken("="),singleValue})
            ->addRule({lexer::getOriginTocken("TO"),
                       lexer::getOriginTocken("STEP"),
                       lexer::getOriginTocken("DRAW"),
                       lexer::getOriginTocken(","),
                       lexer::getOriginTocken(";"),
                       lexer::getOriginTocken(")")},{});

    func->addRule({lexer::getOriginTocken("SIN")},{lexer::getOriginTocken("SIN")})
            ->addRule({lexer::getOriginTocken("COS")},{lexer::getOriginTocken("COS")})
            ->addRule({lexer::getOriginTocken("TAN")},{lexer::getOriginTocken("TAN")})
            ->addRule({lexer::getOriginTocken("LN")},{lexer::getOriginTocken("LN")})
            ->addRule({lexer::getOriginTocken("EXP")},{lexer::getOriginTocken("EXP")})
            ->addRule({lexer::getOriginTocken("SQRT")},{lexer::getOriginTocken("SQRT")});

    matchStack.push(empty);
    matchStack.push(sentence);
    getNextTocken();
}
