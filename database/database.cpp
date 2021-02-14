#include "database.h"

Database::Database(std::string database, unsigned int id) :
    database(database), database_id(id) {}

Database::Database() :
    database("undefined"), database_id(0) {}