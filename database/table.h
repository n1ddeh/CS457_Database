#ifndef TABLE_H_
#define TABLE_H_

#include "include.h"
#include "column.h"

class Table
{
private:
    std::string table_name;                                            // Table name
    std::vector<std::pair<std::string, std::string>> column_meta_data; // Vector of pairs of column_name and column types 
    unsigned int column_count;                                         // Number of columns

    std::map<std::size_t, Column<int>>          INT_COLUMNS;        // DataType: 0 : Stores all INT columns
    std::map<std::size_t, Column<float>>        FLOAT_COLUMNS;      // DataType: 1 : Stores all FLOAT columns
    std::map<std::size_t, Column<char*>>        CHAR_COLUMNS;       // DataType: 2 : Stores all CHAR(_) columns
    std::map<std::size_t, Column<std::string>>  VARCHAR_COLUMNS;    // DataType: 3 : Stores all VARCHAR columns

    std::vector<ColumnData> columns; // Stores column name, type, and ID in order of creation

public:
    Table(std::string table, std::vector<std::pair<std::string, std::string>> column_meta_data);
    ~Table();

    bool insertColumn(std::string column = "undefined", const std::vector<int>&         column_data = std::vector<int>());
    bool insertColumn(std::string column = "undefined", const std::vector<float>&       column_data = std::vector<float>());
    bool insertColumn(std::string column = "undefined", const std::vector<char*>&       column_data = std::vector<char*>());
    bool insertColumn(std::string column = "undefined", const std::vector<std::string>& column_data = std::vector<std::string>());

    bool addColumnToMetaData(std::pair<std::string, std::string> column_info);
    void printColumnMetaData();

    // Getters
    std::string getTable() { return this->table_name; }
    unsigned int getColumnCount() { return this->column_count; }
    // Setters
    void setTable(std::string tableName) { this->table_name = tableName; }
    void incrementColumnCount() { this->column_count++; }
    void decrementColumnCount() { this->column_count--; }
};
#endif //TABLE_H_