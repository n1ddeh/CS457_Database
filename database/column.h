#ifndef COLUMN_H_
#define COLUMN_H_

#include "include.h"

template <class T>
class Column
{
private:
    std::string column_name;        // Column Name
    unsigned int data_type;         // type of elements it stores
    std::vector<T> elements;        // Container for elements
    size_t CHAR_MAX;                // Used for VARCHAR types

public:
    Column(std::string, std::vector<int>);
    Column(std::string, std::vector<float>);
    Column(std::string, std::vector<char>);
    Column(std::string, std::vector<std::string>, size_t);

    bool insertElement(T);

    std::string getName() {return this->column_name;}
    unsigned int getDataType() {return this->data_type;}
    std::vector<T> getElements() {return this->elements;}
    size_t getCharMax() {return this->CHAR_MAX;}
};

#endif // COLUMN_H_