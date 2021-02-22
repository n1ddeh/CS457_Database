#include "database.h"

Database::Database(std::string database, unsigned int id, std::string path) : database(database), database_id(id) 
{
    if (path == std::string("undefined"))
    {
        path = STORAGE_PATH + '/' + database;
    }
}

Database::Database() :
    database("undefined"), database_id(0) {}