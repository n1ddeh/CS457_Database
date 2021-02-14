#include "column.h"

template <class DataType>
Column<DataType>::Column(std::string column, unsigned int dataType, std::vector<DataType>& elements)
    : column(column), dataType(dataType), elements(elements)
{

}

template <class DataType>
Column<DataType>::Column()
    : column("undefined"), dataType(0)
{

}