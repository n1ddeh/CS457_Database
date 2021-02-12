#include "../helper.h"

typedef struct ColumnData
{
    std::string name;
    unsigned int dataType;
    unsigned int id;
} ColumnData;


template <class DataType>
class Column
{
private:
    std::string column; // name of column
    std::string type;   // type of elements it stores
    std::vector<DataType> elements;
    

};
