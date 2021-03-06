/**
 * SQL.h
 * Author: Mark Minkoff
 * Desc: The header file for SQL.h
 * CLI, Commands, Database Management
 */
#ifndef SQL_H_
#define SQL_H_

#include "include.h"
#include "database.h"

class SQL
{
public:
    /**
     *  Default Constructor
     * */
    SQL();
    SQL(std::string);
    ~SQL();

    void initializeCommands();

    /**
     *  SQL_CLI
     * * The active command line interface
     * @return void
    */
    void SQL_CLI();

    
    /**  Handles the command given by the user
     * @param vector<string> args
     * @return bool */
    bool HANDLE_CMD(std::vector<std::string> args);

    /** Checks if a command exists */
    bool cmdExists(const std::string& cmd);

    /** Gets the ID of a command */
    unsigned int cmdId(const std::string& cmd);

    /**  Check if a database is selected
     *  Returns true if a database is selected */
    bool dbSelected();

    /**  Return a shared pointer to a database */
    std::shared_ptr<Database> getDatabase(const std::string& database_name);

    /**  Creates a database and maps it*/
    bool createDatabase(const std::vector<std::string>& args);
    bool createDatabase(const std::string name, const fs::path& path, const fs::path& path_metadata);

    /**  Drops a database*/
    bool dropDatabase(const std::vector<std::string>& args);

    /**  Creates a table (if a db is selected) and maps it*/
    bool createTable(const std::vector<std::string>& args);

    /**  Outputs data from a table  */
    bool selectTable(const std::vector<std::string>& args);
    bool selectAllFromTable(const std::string& table_name);
    bool selectAllQuery(const std::vector<std::string>& args);

    /**  Change a table in some way  */
    bool alterTable(const std::vector<std::string>& args);

    /**  Parse table column parameters for creating a table */
    static std::vector<std::pair<std::string, std::string>> parseTableColumns(std::vector<std::string> columns);

    /**  Drops a table (if a db is selected) and maps it*/
    bool dropTable(const std::vector<std::string>& args);
    
    /**  Sets the selected database*/
    bool useDatabase(std::shared_ptr<Database> db = nullptr);
    bool useDatabase(const std::vector<std::string>& args);

    bool commit(const std::vector<std::string>& args);

    /** Initialized supported column types */
    void initializeTypes();

    /** Given a vector of types, determine if they are supported*/
    bool checkTypes(std::vector<std::string> types);

    /**   dbSelected
     *  Returns true if a database is selected
     *  or false if not */
    bool dbExists(const std::string& database_name);
    
    /** Handles the INSERT INTO command */
    bool insertInto(const std::vector<std::string>& args);

    /** Parses a params string x,y,z,... and puts them in a string vector container*/
    std::vector<std::string> isolateParams(std::string params, char delim = ',');

    /** Handles the UPDATE {{ table_name }} command */
    bool updateTable(const std::vector<std::string>& args);

    /** Handles the DELETE FROM {{ table_name }} command */
    bool deleteFromTable(const std::vector<std::string>& args);

    bool beginTransaction(const std::vector<std::string>& args);

    // database_count getters/mutators
    unsigned int getDatabaseCount() { return this->database_count; }
    void incrementDatabaseCount() { this->database_count++; }
    void decrementDatabaseCount() { this->database_count--; }
    void setDatabaseCount(unsigned int cnt) { this->database_count = cnt; }

    void errorUnknownArguments(const std::vector<std::string>& args, const std::string& cmd, unsigned int index = 0);

    bool readFilesystem();

    // Reads a database metadata file
    const DatabaseMetadata readDatabaseMetadata(const fs::path& path);

    // Reads a table metadata file
    const TableMetadata readTableMetadata(const fs::path& path);

    // Reads a csv file and stores it in a table
    bool readCSV(std::shared_ptr<Table> table, const fs::path& path);

private:
    std::unordered_map<std::string, std::shared_ptr<Database>> databases;   // Database Storage <database_name, database*>
    std::unordered_map<std::string, unsigned int> commands;                 // Command Storage <command_name, command_id>
    std::unordered_map<std::string, unsigned int> types;                    // Supported column Types <type_name, id>
    std::shared_ptr<Database> database;                                     // The selected database
    unsigned int database_count;                                            // The number of stored databases
    std::queue<std::string> arguments;
    std::string process_id;
    std::queue<std::string> transactionArguments;
};

#endif
