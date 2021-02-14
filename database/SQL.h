#ifndef SQL_H_
#define SQL_H_

#include "database.h"
#include "../commands/cmd.h"

class SQL
{
public:
    // Constructor
    SQL();
    ~SQL();

    void readUserInput();

    /**
     * split
     * * Split a string by a delimiter and return a vector of strings
     * @param string The string being split
     * @param char The delimiter to split by
     * @return vector<string> The split string
    */
    std::vector<std::string> split(const std::string& s, char delimiter);

private:
    // stores known databases
    std::unordered_map<std::string, Database> databases;

    // Selected Database
    Database* database;
    bool db_selected;
    

};

#endif