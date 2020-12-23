#ifndef PARSER_H
#define PARSER_H
#include "../lexer/tocken.h"

/*
 * command -> sentence ;
 * sentence -> KEYWORD
 * sentence -> setOrigin
 *          -> setRotation
 *          -> forDraw
 *          -> defineSymbol
 *          -> assinValue
 * setOrigin -> ORIGIN VAL
 * setRotation -> ROT VAL
 * forDraw -> FOR VAL TO VAL STEP VAL DRAW
 * defineSymbol -> DEF SYMBOL
 *              -> DEF SYMBOL = VAL
 *              -> DEF SYMBOL IS VAL
 *
 * assinValue -> SYMBOL = VAL
 *            -> SYMBOL IS VAL
 * VAL -> FUNC(VAL)
 *     -> (VAL,VAL)
 *     -> NUMBER
 *     -> SYMBOL
 *     -> CONST_ID
 */


class parser
{
private:
    QList<tocken> tockens;
    QVector<tocken> tockenPairStack;
    QList<tocken>::iterator currentTocken;

    void getNextTocken();// 从表达式串中获取下一个tocken
    void putTockenback();// 将当前tocken退回输入流
    bool match(const tocken& _tocken);// 匹配当前tocken
    bool matchPair(const tocken& _tocken);// 匹配成对出现的tocken
public:
    parser();
};

#endif // PARSER_H
