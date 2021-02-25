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
    bool tableExists(const std::string& table_name);
};

#endif // DATABASE_H_