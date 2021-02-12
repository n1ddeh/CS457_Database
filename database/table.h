#include "../helper.h"
#include "column.h"

namespace DB
{
class Table
{
private:
    std::string table;      // Name of Table
    unsigned int table_id;  
    unsigned int col_count; 

    std::map<std::size_t, Column<int>>          INT_COLUMNS;        // DataType: 0
    std::map<std::size_t, Column<float>>        FLOAT_COLUMNS;      // DataType: 1
    std::map<std::size_t, Column<const char[]>> CHAR_COLUMNS;       // DataType: 2
    std::map<std::size_t, Column<std::string>>  VARCHAR_COLUMNS;    // DataType: 3

    std::vector<ColumnData> columns; // Stores column name, type, and ID in order of creation

public:
    bool insertIntColumn(std::string column = "undefined", const std::vector<int>& column_data = std::vector<int>())
    {
        
    }
    bool insertFloatColumn(std::string column = "undefined", const std::vector<float>& column_data = std::vector<float>())
    {
        
    }
    bool insertCharColumn(std::string column = "undefined", const std::vector<const char[]>& column_data = std::vector<const char[]>())
    {
        
    }
    bool insertVarCharColumn(std::string column = "undefined", const std::vector<std::string>& column_data = std::vector<std::string>())
    {
        
    }

    // Getters
    std::string getTable()
    {
        return this->table;
    }
    unsigned int getTableID()
    {
        return this->table_id;
    }
    unsigned int getColumnCount()
    {
        return this->col_count;
    }
    // Setters
    void setTable(std::string tableName)
    {
        this->table = tableName;
    }
    void setTableID(unsigned int id)
    {
        this->table_id = id;
    }
    void incrementColumnCount()
    {
        this->col_count++;
    }
    void decrementColumnCount()
    {
        this->col_count--;
    }


};


}