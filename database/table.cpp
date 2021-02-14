#include "table.h"

// Constructor
Table::Table(std::string table, unsigned int id) : 
    table(table), table_id(id), col_count(0) {}

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