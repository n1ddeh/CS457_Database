#include "SQL.h"

SQL::SQL() : database(nullptr), database_count(0)
{
    SQL::SQL_CLI();


}

void SQL::SQL_CLI()
{
    std::string input;
    
    std::getline(std::cin, input);

    std::vector<std::string> args = SQL::split(input, ' ');

    CMD * cmd = new CMD(args);

    return SQL_CLI();
}

std::vector<std::string> SQL::split(const std::string& s, char delimiter) 
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

bool SQL::dbSelected()
{
    if (this->database) {
        return true;
    }
    return false;
}

bool SQL::createDatabase(std::string database, unsigned int database_id, std::string path)
{
    try
    {
        Database* db = new Database(database, this->database_count + 1, path);

        this->databases.emplace_back(db);

        this->database_count++;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    
    return true;
}

Database* SQL::useDatabase(Database* db)
{
    if (db == nullptr) return nullptr;

    this->database = db;

    return this->database;
}