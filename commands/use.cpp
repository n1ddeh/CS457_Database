#include "use.h"

Use::Use()
{
    std::cout << "Use default constructor called\n";
}

Use::Use(std::vector<std::string>& arguments)
{   
    std::cout << "Use arg constructor called\n";
}

Use::~Use()
{
    std::cout << "Use destructor called\n";
}