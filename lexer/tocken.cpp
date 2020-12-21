#include "tocken.h"

int tocken::match(const QString & str)
{
    return matcher->match(str);
}
