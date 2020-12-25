#ifndef MYTOCKENS_H
#define MYTOCKENS_H

#include "tockensDefine.h"

class myTockens:public tockenDefine
{
public:
    myTockens(){loadDefination();};

public:
    void loadDefination() override;
};

#endif // MYTOCKENS_H
