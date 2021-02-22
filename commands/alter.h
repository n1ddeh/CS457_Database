#ifndef ALTER_H_
#define ALTER_H_

#include "cmd.h"

class Alter: public CMD
{
public:
    Alter();
    Alter(std::vector<std::string>& arguments);
    virtual ~Alter();
};

#endif