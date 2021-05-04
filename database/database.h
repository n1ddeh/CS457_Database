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
class Database
{
private:
    std::string database_name;                                      // Database Name
    std::unordered_map<std::string, std::shared_ptr<Table>> tables; // Tables within the Database
    fs::path path;                                                  // The path to the database folder
    fs::path path_metadata;
    bool transaction_mode;

public:
    Database();
    Database(const std::string& database, const fs::path& path, const fs::path& path_metadata);
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

    bool setTransaction(bool val) {
        this->transaction_mode = val;
        this->writeMetadata();
        return this->transaction_mode;
    }

    bool getTransaction() {
        return this->transaction_mode;
    }

    fs::path getPath() {
        return this->path;
    }

    fs::path setPath(const fs::path& path) {
        this->path = path;
        return this->path;
    }

    fs::path getPathMetadata() {
        return this->path_metadata;
    }

    fs::path setPathMetadata(const fs::path& path) {
        this->path_metadata = path;
        return this->path_metadata;
    }

    // Write private variables to files system
    bool writeMetadata();
    bool writeMetadata(const DatabaseMetadata& md );

    // Read metadata from filesystem
    const DatabaseMetadata readMetadata();

    // Set private variables in memory to database metadata
    void applyMetadata(const DatabaseMetadata& md );

    // Get metadata from memory
    const DatabaseMetadata getMetadata();

    std::unordered_map<std::string, std::shared_ptr<Table>> getTables() {
        return this->tables;
    }
};

#endif // DATABASE_H_
