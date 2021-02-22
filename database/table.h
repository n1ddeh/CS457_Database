#ifndef TABLE_H_
#define TABLE_H_
#include "column.h"

class Table
{
private:
    std::string table;      // Table name
    std::string path;       // Path of table
    unsigned int table_id;  // ID of table (incremental)
    unsigned int col_count; // Number of columns

    std::map<std::size_t, Column<int>>          INT_COLUMNS;        // DataType: 0 : Stores all INT columns
    std::map<std::size_t, Column<float>>        FLOAT_COLUMNS;      // DataType: 1 : Stores all FLOAT columns
    std::map<std::size_t, Column<char*>>        CHAR_COLUMNS;       // DataType: 2 : Stores all CHAR(_) columns
    std::map<std::size_t, Column<std::string>>  VARCHAR_COLUMNS;    // DataType: 3 : Stores all VARCHAR columns

    std::vector<ColumnData> columns; // Stores column name, type, and ID in order of creation

public:
    Table(std::string table = "undefined", unsigned int id = 0);
    ~Table();

    bool insertColumn(std::string column = "undefined", const std::vector<int>& column_data = std::vector<int>());
    bool insertColumn(std::string column = "undefined", const std::vector<float>& column_data = std::vector<float>());
    bool insertColumn(std::string column = "undefined", const std::vector<char*>& column_data = std::vector<char*>());
    bool insertColumn(std::string column = "undefined", const std::vector<std::string>& column_data = std::vector<std::string>());

    // Getters
    std::string getTable() {
        return this->table;
    }
    unsigned int getTableID() {
        return this->table_id;
    }
    unsigned int getColumnCount() {
        return this->col_count;
    }
    // Setters
    void setTable(std::string tableName) {
        this->table = tableName;
    }
    void setTableID(unsigned int id) {
        this->table_id = id;
    }
    void incrementColumnCount() {
        this->col_count++;
    }
    void decrementColumnCount()
{
        this->col_count--;
    }


};

#endif //TABLE_H_