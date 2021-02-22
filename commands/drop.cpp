#include "drop.h"

Drop::Drop()
{
    std::cout << "Drop default constructor called\n";
}

Drop::Drop(std::vector<std::string>& arguments)
{
    std::cout << "Drop arg constructor called\n";
}

Drop::~Drop()
{
    std::cout << "Drop destructor called\n";
}