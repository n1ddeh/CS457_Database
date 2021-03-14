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

    /**  Drops a database*/
    bool dropDatabase(const std::vector<std::string>& args);

    /**  Creates a table (if a db is selected) and maps it*/
    bool createTable(const std::vector<std::string>& args);

    /**  Outputs data from a table  */
    bool selectTable(const std::vector<std::string>& args);
    bool selectAllFromTable(const std::string& table_name);

    /**  Change a table in some way  */
    bool alterTable(const std::vector<std::string>& args);

    /**  Parse table column parameters for creating a table */
    static std::vector<std::pair<std::string, std::string>> parseTableColumns(std::vector<std::string> columns);

    /**  Drops a table (if a db is selected) and maps it*/
    bool dropTable(const std::vector<std::string>& args);
    
    /**  Sets the selected database*/
    bool useDatabase(std::shared_ptr<Database> db = nullptr);
    bool useDatabase(const std::vector<std::string>& args);

    /**   dbSelected
     *  Returns true if a database is selected
     *  or false if not */
    bool dbExists(const std::string& database_name);

    // database_count getters/mutators
    unsigned int getDatabaseCount() { return this->database_count; }
    void incrementDatabaseCount() { this->database_count++; }
    void decrementDatabaseCount() { this->database_count--; }
    void setDatabaseCount(unsigned int cnt) { this->database_count = cnt; }

    /**  * Split a string by a delimiter and return a vector of strings
     * @param string The string being split
     * @param char The delimiter to split by
     * @return vector<string> The split string */
    static std::vector<std::string> split(const std::string& s, char delimiter);

    /**  * Check if a parenthesis are balanced in a string. 
     *  ! Credit goes to chammika at leetcode.com
     * * https://leetcode.com/problems/valid-parentheses/discuss/9252/2ms-C%2B%2B-sloution
     * @param string The string being split
     * @return bool */
    bool parenthesisBalance(std::string s);

    void errorUnknownArguments(const std::vector<std::string>& args, const std::string& cmd, unsigned int index = 0);

private:
    std::unordered_map<std::string, std::shared_ptr<Database>> databases;   // Database Storage <database_name, database*>
    std::unordered_map<std::string, unsigned int> commands;                 // Command Storage <command_name, command_id>
    std::shared_ptr<Database> database;                                     // The selected database
    unsigned int database_count;                                            // The number of stored databases
};


#endif