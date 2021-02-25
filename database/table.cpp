#include "table.h"

// Constructor
Table::Table(std::string table, std::vector<std::pair<std::string, std::string>> column_meta_data) : 
    table_name(table), column_count(0), column_meta_data(column_meta_data) 
{
    std::cout << "Table " << this->table_name << " created.\n";
}

Table::~Table() 
{
    std::cout << "Table " << this->table_name << "deleted.\n";
}

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