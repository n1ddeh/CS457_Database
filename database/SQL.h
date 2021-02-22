#ifndef SQL_H_
#define SQL_H_

#include "database.h"
#include "../commands/cmd.h"

class SQL
{
public:
    /**
     *  Default Constructor
     * */
    SQL();
    ~SQL();

    /**
     *  SQL_CLI
     * * The active command line interface
     * @return void
    */
    void SQL_CLI();

    /**
     *  Handels the command given by the user
     * @param vector<string> args
     * @return bool
    */
    bool HANDLE_CMD(std::vector<std::string>& args);
    

    /**
     * split
     * * Split a string by a delimiter and return a vector of strings
     * @param string The string being split
     * @param char The delimiter to split by
     * @return vector<string> The split string
    */
    std::vector<std::string> split(const std::string& s, char delimiter);

    /** 
     *  dbSelected
     *  Returns true if a database is selected
     *  or false if not
    */
    bool dbSelected();

    bool createDatabase(std::string database = "undefined", unsigned int database_id = 0, std::string path = "undefined");

    Database* useDatabase(Database* db = nullptr);


private:
    /**
     *  Database Storage
    */
    std::vector<Database*> databases;

    // The selected database
    Database* database;

    // The number of stored databases
    unsigned int database_count;
};

#endif