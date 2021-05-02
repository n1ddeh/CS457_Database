/**
 * File: table.h
 * Author: Mark Minkoff
 * Functionality: Function declarations for file table.cpp
 * 
 * */

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
    fs::path path;                                                     // The path to the table file
    fs::path path_metadata;
    bool locked;                                                       // Determines if changes can be made to the table

    // Storage container for each column
    std::vector<std::variant<std::shared_ptr<Column<int>>, std::shared_ptr<Column<float>>, std::shared_ptr<Column<char>>, std::shared_ptr<Column<std::string>>>> columns;

public:
    /** Standard Table Constructor 
     * @param std::string table_name
     * @param std::vector<std::pair<std::string, std::string>> column_meta_data
    */
    Table(
        std::string table, 
        std::vector<std::pair<std::string, std::string>> column_meta_data,
        fs::path path,
        fs::path path_metadata
    );
    /** Table Destructor */
    ~Table();

    // ---------------------------
    // ---- Table Creation Functions
    // ---------------------------

    /** Creates an INT Column */
    bool createIntColumn(std::string);

    /** Creates a FLOAT Column */
    bool createFloatColumn(std::string);

    /** Creates a CHAR Column */
    bool createCharColumn(std::string);

    /** Creates a VarChar Column */
    bool createVarCharColumn(std::string, size_t);
    
    // ---------------------------
    // ---- Table Update Functions
    // ---------------------------

    /** Handels the UPDATE {{ table_name }} SET Command */
    bool updateColumnSet(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);

    /** Handles the DELETE FROM {{ table_anme }} */
    bool deleteFromTable(const std::string&, const std::string&, const std::string&);

    /** Handles the INSERT INTO {{ table_name }} VALUES(x, y, z, ...) Command */
    bool insertRow(const std::vector<std::string>&);

    /**  Deletes a from the table based on index*/
    bool deleteRow(const size_t);

    bool writeMetadata();

    // ---------------------------
    // ---- Table Selection Functions
    // ---------------------------

    /** Handles the SELECT {{ col1, col2, ... }} FROM {{ table_name }} WHERE command */
    bool selectColumns(
        const std::vector<std::string>& columns,
        const std::string& column_to_query,
        const std::string& value_to_query, 
        const std::string& opr
    );

    std::shared_ptr<Column<int>>         selectColumnInt   (const std::string& column_name);
    std::shared_ptr<Column<float>>       selectColumnFloat (const std::string& column_name);
    std::shared_ptr<Column<char>>        selectColumnChar  (const std::string& column_name);
    std::shared_ptr<Column<std::string>> selectColumnString(const std::string& column_name);

    /** Handles the SELECT * command */
    bool printAll();

    // ---------------------------
    // ---- Table Helper Functions
    // ---------------------------

    /** Returns an index to a column based on the name passed */
    long int columnIndexFromName(const std::string&);

    /** Checks if a column exists*/
    bool columnExists(const std::string& column_name);

    bool printRow(const size_t row);

    // Getters
    std::string getTable() { return this->table_name; }
    unsigned int columnCount() { return this->column_count; }
    size_t getColumnType(const std::string&);
    const fs::path getPath() { return this->path; }
    const fs::path getPathMetadata() { return this->path_metadata; }
    std::vector<std::pair<std::string, std::string>> getMetaData() { return this->column_meta_data; }
    bool getLocked() { return this->locked; }
    
    // Setters
    void setTableName(const std::string& name) { this->table_name = name; }
    void incrementColumnCount() { this->column_count++; }
    void decrementColumnCount() { this->column_count--; }
    void incrementRowCount() { this->row_count++; }
    void decrementRowCount() { this->row_count--; }
    void setLocked(bool val) { this->locked = val; }
};
#endif //TABLE_H_
