#ifndef PARSER_H
#define PARSER_H
#include "../lexer/tocken.h"
#include "parseelement.h"

/*
 * sentence -> command ; sentence
 *          -> EMPTY
 *
 * command  -> setOrigin
 *          -> setRotation
 *          -> forDraw
 *          -> defineSymbol
 *          -> assignValue
 *
 * setOrigin -> ORIGIN IS COORDINATE
 *
 * setRotation -> ROT IS SINGLEVAL
 *
 * forDraw -> FOR SIMBOL FROM SINGLEVAL TO SINGLEVAL STEP SINGLEVAL DRAW COORDINATE
 *
 * defineSymbol -> DEF SYMBOL           ------> defineSymbol -> DEF SYMBOL assign1
 *              -> DEF SYMBOL = VAL             assign1 -> =VAL
 *              -> DEF SYMBOL IS VAL                    -> IS VAL
 *                                                      -> EMPTY
 *
 * assignValue -> SYMBOL = VAL          ------> assignValue -> SYMBOL assign
 *            -> SYMBOL IS VAL                  assign  -> =VAL
 *                                                      -> IS VAL
 * VAL -> SINGLEVAL
 *     -> COORDINATE
 *
 * SINGLEVAL -> EXPR       =======>  SINGLEVAL -> CONST_ID EXPR'
 *           -> CONST_ID                       -> SYMBOL EXPR'
 *           -> SYMBOL                         -> NUMBER EXPR'
 *           -> NUMBER                         -> FUNC(VAL) EXPR'
 *           -> FUNC(VAL)                      -> (SINGLEVAL)
 *           -> (SINGLEVAL)
 *
 * COORDINATE -> (SINGLEVAL,SINGLEVAL)
 *
 * EXPR -> SINGLEVAL+SINGLEVAL
 *      -> SINGLEVAL-SINGLEVAL
 *      -> SINGLEVAL*SINGLEVAL
 *      -> SINGLEVAL/SINGLEVAL
 *      -> SINGLEVAL==SINGLEVAL
 *
 * EXPR' -> +SINGLEVAL
 *       -> -SINGLEVAL
 *       -> *SINGLEVAL
 *       -> /SINGLEVAL
 *       -> ==SINGLEVAL
 *       -> EMPTY
 *
 * FUNC -> SIN|COS|TAN|LN|EXP|SQRT
 */


class parser
{
private:
    QVector<QSharedPointer<tocken>> ruleTable;// 驱动规则表

    QList<QSharedPointer<tocken>> tockens;//输入tocken列表

    int index=-1;
    QSharedPointer<tocken> currentTocken;//当前tocken

    QStack<QSharedPointer<tocken>> matchStack;//符号栈

    void getNextTocken();
    void putTockenback();
public:
    parser(QList<QSharedPointer<tocken>> _tockens);
    bool analyze();//表驱动分析
};

#endif // PARSER_H
