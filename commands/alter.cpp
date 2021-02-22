#include "alter.h"

Alter::Alter() 
{
    std::cout << "Alter default constructor called\n";
}

Alter::Alter(std::vector<std::string>& arguments) 
{
    std::cout << "Alter arg constructor called\n";
}

Alter::~Alter() 
{
    std::cout << "Alter destructor called\n";
}