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
    unsigned int row_count;                                            // number of rows

    std::vector<std::variant<std::shared_ptr<Column<int>>, std::shared_ptr<Column<float>>, std::shared_ptr<Column<char>>, std::shared_ptr<Column<std::string>>>> columns;

public:
    Table(std::string table, std::vector<std::pair<std::string, std::string>> column_meta_data);
    ~Table();

    bool createIntColumn(std::string);
    bool createFloatColumn(std::string);
    bool createCharColumn(std::string);
    bool createVarCharColumn(std::string, size_t);

    bool updateColumnSet(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
    bool deleteFromTable(const std::string&, const std::string&, const std::string&);

    bool insertRow(const std::vector<std::string>&);

    bool deleteRow(const size_t);

    bool printAll();

    long int columnIndexFromName(const std::string&);

    

    // Getters
    std::string getTable() { return this->table_name; }
    unsigned int columnCount() { return this->column_count; }
    // Setters
    void setTableName(const std::string& name) { this->table_name = name; }
    void incrementColumnCount() { this->column_count++; }
    void decrementColumnCount() { this->column_count--; }
    void incrementRowCount() { this->row_count++; }
    void decrementRowCount() { this->row_count--; }
};
#endif //TABLE_H_