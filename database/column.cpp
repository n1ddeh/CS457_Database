#include "column.h"

template<> Column<int>::Column(std::string column, std::vector<int> elements)
{
    this->column_name = column;
    this->elements = elements;
    this->data_type = 0;
}

template<> Column<float>::Column(std::string column, std::vector<float> elements)
{
    this->column_name = column;
    this->elements = elements;
    this->data_type = 1;
}

template<> Column<char>::Column(std::string column, std::vector<char> elements)
{
    this->column_name = column;
    this->elements = elements;
    this->data_type = 2;
}

template<> Column<std::string>::Column(std::string column, std::vector<std::string> elements, size_t max)
{
    this->column_name = column;
    this->elements = elements;
    this->data_type = 3;
    this->CHAR_MAX = max;
}

template<> bool Column<int>::insertElement(int el)
{
    try 
    {
        this->elements.emplace_back(el);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return false;
    }
    return true;
}

template<> bool Column<float>::insertElement(float el)
{
    try 
    {
        this->elements.emplace_back(el);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return false;
    }
    return true;
}

template<> bool Column<char>::insertElement(char el)
{
    try 
    {
        this->elements.emplace_back(el);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return false;
    }
    return true;
}

template<> bool Column<std::string>::insertElement(std::string el)
{
    try 
    {
        this->elements.emplace_back(el);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return false;
    }
    return true;
}
