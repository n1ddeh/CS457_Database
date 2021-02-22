#include "create.h"
#include "cmd.h"

Create::Create() 
{
    std::cout << "Create default constructor called\n";
}

Create::Create(std::vector<std::string>& arguments)
{
    std::cout << "Create arg constructor called\n";
}


Create::~Create()
{
    std::cout << "Create Destructor called\n";
}