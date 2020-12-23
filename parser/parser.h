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
public:
    parser();
};

#endif // PARSER_H
