#ifndef USE_H_
#define USE_H_

#include "cmd.h"
class Use: public CMD
{
public:
    Use();
    Use(std::vector<std::string>& arguments);
    virtual ~Use();
};

#endif