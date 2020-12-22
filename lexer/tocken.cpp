#include "tocken.h"

void tocken::castType()
{
    QString str="%1";
    switch (type)
    {
    case FUNC:
        str = str.arg("FUNC");
        break;
    case KEYWORD:
        str = str.arg("KEYWORD");
        break;
    case OP:
        str = str.arg("OPERATOR");
        break;
    case MARK:
        str = str.arg("MARK");
        break;
    case END:
        str = str.arg("END");
        break;
    case EMPTY:
        str = str.arg("EMPTY");
        break;
    case NUMBER:
        str = str.arg("NUMBER");
        break;
    case CONST_ID:
        str = str.arg("CONST_ID");
        break;
    case SYMBOL:
        str = str.arg("SYMBOL");
    }
    property["type"] = str;
}

int tocken::match(const QString & str)
{
    return matcher->match(str);
}
