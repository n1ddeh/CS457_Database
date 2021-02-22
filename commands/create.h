#ifndef CREATE_H_
#define CREATE_H_

#include "cmd.h"
class Create: public CMD
{
public:
    Create();
    Create(std::vector<std::string>& arguments);
    virtual ~Create();
};

#endif