#ifndef COLUMN_H_
#define COLUMN_H_

#include "include.h"

/**
 * struct ColumnData
 * * metadata for columns
 * */
typedef struct ColumnData
{
    std::string name;
    unsigned int dataType;
    unsigned int id;

    ColumnData(std::string name, unsigned int dataType, unsigned int id) :
        name(name), dataType(dataType), id(id) {}

} ColumnData;

template <class DataType>
class Column
{
private:
    std::string column; // name of column
    std::string dataType;   // type of elements it stores
    std::vector<DataType> elements;
    unsigned int CHAR_MAX;

public:
    Column();
    Column(std::string column = "undefined", unsigned int dataType = 0, std::vector<DataType>& elements = new std::vector<DataType>());
};

#endif // COLUMN_H_