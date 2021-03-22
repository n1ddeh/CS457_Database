#include "database.h"

Database::Database(const std::string& database) : database_name(database) {}

Database::Database() : database_name("undefined") {}

Database::~Database() {}

bool Database::tableExists(const std::string& table_name)
{
    return (bool) this->tables.count(table_name);
}

bool Database::createTable(std::string table_name, std::vector<std::pair<std::string, std::string>> columns)
{
    std::shared_ptr<Table> new_table = std::make_shared<Table>(table_name, columns);

    this->tables.insert(std::make_pair(table_name, new_table));
    
    return true;
}

bool Database::dropTable(const std::string& table_name)
{
    if (tableExists(table_name))
    {
        this->tables.erase(table_name);
        return true;
    }
    return false;
}

bool Database::printTableColumnInfo(const std::string& table_name)
{
    if (tableExists(table_name))
    {
        std::shared_ptr<Table> table = getTable(table_name);
        
        return true;
    }
    return false;
}

std::shared_ptr<Table> Database::getTable(const std::string& table_name)
{
    if (tableExists(table_name))
    {
        return this->tables.at(table_name);
    }
    return nullptr;
}

bool Database::addColumnsToTable(const std::string& table_name, std::vector<std::pair<std::string, std::string>> columns)
{
    std::shared_ptr<Table> table = getTable(table_name);
    for (int i = 0; i < columns.size(); i++)
    {
        
    }
    return true;
}