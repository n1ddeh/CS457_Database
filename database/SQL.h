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
    bool HANDLE_CMD(const std::vector<std::string>& args);

    /** Checks if a command exists */
    bool cmdExists(const std::string& cmd);

    /** Gets the ID of a command */
    unsigned int cmdId(const std::string& cmd);

    /**  Check if a database is selected
     *  Returns true if a database is selected */
    bool dbSelected();

    /**  Creates a database and maps it*/
    bool createDatabase(const std::vector<std::string>& args);

    /**  Drops a database*/
    bool dropDatabase(const std::vector<std::string>& args);

    /**  Creates a table (if a db is selected) and maps it*/
    bool createTable(const std::vector<std::string>& args);

    /**  Drops a table (if a db is selected) and maps it*/
    bool dropTable(const std::vector<std::string>& args);
    
    /**  Sets the selected database*/
    std::shared_ptr<Database> useDatabase(std::shared_ptr<Database> db = nullptr);

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
    std::vector<std::string> split(const std::string& s, char delimiter);

    void errorUnknownArguments(const std::vector<std::string>& args, const std::string& cmd, unsigned int index = 0);

private:
    std::unordered_map<std::string, std::shared_ptr<Database>> databases;   // Database Storage <database_name, database*>
    std::unordered_map<std::string, unsigned int> commands;                 // Command Storage <command_name, command_id>
    std::shared_ptr<Database> database;                                     // The selected database
    unsigned int database_count;                                            // The number of stored databases
};


#endif