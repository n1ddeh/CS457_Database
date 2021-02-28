#include "table.h"

// Constructor
Table::Table(std::string table, std::vector<std::pair<std::string, std::string>> column_meta_data) : 
    table_name(table), column_count(0), column_meta_data(column_meta_data) {}

Table::~Table() {}

// Insert Int column
bool Table::insertColumn(std::string column, const std::vector<int>& column_data) 
{

    return true;
}

// Insert Float Column
bool Table::insertColumn(std::string column, const std::vector<float>& column_data) 
{

    return true;
}

// Inserted Char Column
bool Table::insertColumn(std::string column, const std::vector<char*>& column_data) 
{


    return true;
}

// Insert Varchar Column
bool Table::insertColumn(std::string column, const std::vector<std::string>& column_data) 
{

    return true;
}

void Table::printColumnMetaData()
{
    for (int i = 0; i < this->column_meta_data.size(); i++)
    {
        std::cout << std::get<0>(this->column_meta_data[i]) << " " << std::get<1>(this->column_meta_data[i]);
        if (i != (this->column_meta_data.size() - 1)) std::cout << " | ";
    }
    std::cout << "\n"; 
}

bool Table::addColumnToMetaData(std::pair<std::string, std::string> column_info)
{
    const std::string column_name = std::get<0>(column_info);
    const std::string column_type = std::get<1>(column_info);
    
    this->column_meta_data.push_back(column_info);

    return true;
}