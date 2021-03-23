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

public:
    Database();
    Database(const std::string& database = "undefined");
    ~Database();
    
    bool createTable(std::string table_name, std::vector<std::pair<std::string, std::string>> columns);
    bool dropTable(const std::string& table_name);
    bool addColumnsToTable(const std::string& table_name, std::vector<std::pair<std::string, std::string>> columns);

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
};

#endif // DATABASE_H_