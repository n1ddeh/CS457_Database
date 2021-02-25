#include "database.h"

Database::Database(const std::string& database) : database_name(database) {
    std::cout << "Database " << database << " created.\n";
}

Database::Database() : database_name("undefined") {}

Database::~Database() {
    std::cout << "Database " << this->database_name << " deleted.\n";
}

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