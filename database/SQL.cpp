#include "SQL.h"
#include "../commands/create.h"
#include "../commands/drop.h"
#include "../commands/use.h"
#include "../commands/alter.h"

SQL::SQL() : database(nullptr), database_count(0)
{
    SQL::SQL_CLI();
}

SQL::~SQL()
{

}

void SQL::SQL_CLI()
{
    std::string input;
    
    std::getline(std::cin, input);

    std::vector<std::string> args = SQL::split(input, ' ');

    HANDLE_CMD(args);

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

bool SQL::HANDLE_CMD(std::vector<std::string>& args)
{
    std::cout << "Command Handler called\n";
    try {
        // Grab the commands from the provided arguments
        std::string command = args[0];

        CMD* protoype = new CMD();

        // Check if command exists, if it doesn't return 0
        if (!protoype->checkCMD(command)) {
            std::cout << "Command " << command << " does not exist.\n";
            return 0;
        }

        unsigned int command_id = protoype->get_CMD_ID(command);

        if (command_id == 0)
        {
            Create* createCMD = new Create(args);
            delete createCMD;
        }
        else if (command_id == 1)
        {
            Drop* dropCMD = new Drop(args);
            delete dropCMD;
        }
        else if (command_id == 2)
        {
            Use* useCMD = new Use(args);
            delete useCMD;
        }
        else if (command_id == 3)
        {
            Alter* alterCMD = new Alter(args);
            delete alterCMD;
        }

        delete protoype;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
    }

    return 1;
}