#include "../helper.h"
#include "table.h"

namespace DB {
class Database
{
private:
    std::string database; 
    std::string path;
    std::vector<Table> tables;
    
};
}
