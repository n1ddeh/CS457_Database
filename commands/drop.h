#ifndef DROP_H_
#define DROP_H_

#include "cmd.h"
class Drop: public CMD
{
public:
    Drop();
    Drop(std::vector<std::string>& arguments);
    virtual ~Drop();
};

#endif