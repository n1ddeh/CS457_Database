#ifndef DATABASE_H_
#define DATABASE_H_

#include "table.h"

#define STORAGE_PATH std::string("../storage")
class Database
{
private:
    std::string database;       // Database Name
    unsigned int database_id;   // Database ID
    std::string path;           // Storage Path of Database
    std::vector<Table*> tables;  // Tables within the Database

public:
    Database();

    // Database Constructor
    Database(std::string database = "undefined", unsigned int id = 0, std::string path = "undefined");

    // Getters
    std::string getDatabase() {
        return this->database;
    }
    unsigned int getDatabaseId() {
        return this->database_id;
    }
};

#endif // DATABASE_H_