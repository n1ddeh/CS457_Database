/**
 * File: database.h
 * Author: Mark Minkoff
 * Functionality: Function declerations for file column.cpp
 * 
 * */
#ifndef DATABASE_H_
#define DATABASE_H_

#include "include.h"
#include "table.h"

#define STORAGE_PATH std::string("../storage")
class Database
{
private:
    std::string database_name;                                      // Database Name
    std::unordered_map<std::string, std::shared_ptr<Table>> tables; // Tables within the Database
    fs::path path;                                      // The path to the database folder

public:
    Database();
    Database(const std::string& database, const fs::path& path);
    ~Database();
    
    bool createTable(std::string table_name, std::vector<std::pair<std::string, std::string>> columns);
    bool dropTable(const std::string& table_name);
    bool addColumnsToTable(const std::string& table_name, std::vector<std::pair<std::string, std::string>> columns);

    bool queryTables(
        const std::pair<std::string, std::string>& table1,
        const std::pair<std::string, std::string>& table2,
        const std::pair<bool, bool>& lr_val,
        const bool inner,
        const std::vector<std::string>& statement
    );

    /**
     *  Check if a table exists
     * 
     * @param const string
     * @return bool
     * */
    bool tableExists(const std::string& table_name);

    /**
     *  Print column metadata of a table
     * 
     * @param const string
     * @return bool (true if success)
     * */
    bool printTableColumnInfo(const std::string& table_name);

    /**  Get the name of the database
     * 
     * @return string 
     * */
    std::string getDatabaseName() { return this->database_name; }


    void setDatabaseName(std::string db) { this->database_name = db; }
    std::shared_ptr<Table> getTable(const std::string& table_name);

    std::unordered_map<size_t, std::vector<size_t>> queryColumnsInt(
        std::shared_ptr<Column<int>> col1, 
        std::shared_ptr<Column<int>> col2, 
        const std::string& op
    );

    std::unordered_map<size_t, std::vector<size_t>> queryColumnsFloat(
        std::shared_ptr<Column<float>> col1, 
        std::shared_ptr<Column<float>> col2, 
        const std::string& op
    );

    std::unordered_map<size_t, std::vector<size_t>> queryColumnsChar(
        std::shared_ptr<Column<char>> col1, 
        std::shared_ptr<Column<char>> col2, 
        const std::string& op
    );
    
    std::unordered_map<size_t, std::vector<size_t>> queryColumnsString(
        std::shared_ptr<Column<std::string>> col1, 
        std::shared_ptr<Column<std::string>> col2, 
        const std::string& op
    );

    bool printQuery(
        std::shared_ptr<Table> table1, 
        std::shared_ptr<Table> table2,
        std::unordered_map<size_t, std::vector<size_t>> map1,
        std::unordered_map<size_t, std::vector<size_t>> map2,
        bool inner
    );
};

#endif // DATABASE_H_
