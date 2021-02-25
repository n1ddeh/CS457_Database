#include "SQL.h"

SQL::SQL() : database_count(0)
{
    initializeCommands();
    SQL_CLI();
}

SQL::~SQL()
{
    std::cout << "All done.\n";
}

void SQL::initializeCommands()
{
    std::size_t NUM_COMMANDS = 4;

    this->commands.reserve(NUM_COMMANDS);

    std::pair<std::string, unsigned int> cmd1("CREATE", 0);
    std::pair<std::string, unsigned int> cmd2("DROP",   1);
    std::pair<std::string, unsigned int> cmd3("USE",    2);
    std::pair<std::string, unsigned int> cmd4("ALTER",  3);

    this->commands.insert(cmd1);
    this->commands.insert(cmd2);
    this->commands.insert(cmd3); 
    this->commands.insert(cmd4); 
}

void SQL::SQL_CLI()
{
    std::string input;
    std::getline(std::cin, input);
    std::vector<std::string> args = split(input, ' ');
    HANDLE_CMD(args);

    return SQL_CLI();
}

bool SQL::dbSelected()
{
    if (this->database != nullptr) {
        return true;
    }
    return false;
}

bool SQL::createDatabase(const std::vector<std::string>& args)
{

    const unsigned int argn = args.size();
    const unsigned int max_argn = 3;
    
    if (argn < max_argn) { std::cout << "[CMD - CREATE - ERROR] -> Supplied argument count (" << argn << ") does not match required argument count (" << max_argn << ")\n"; return false; }
    
    const std::string command_name  = args[0];
    const std::string database      = args[1];
    const std::string database_name = args[2];

    // Check that the arguments are correct
    if (command_name != "CREATE" || database != "DATABASE") 
    { 
        std::cout << "Programmer error in SQL.cpp -> createDatabase. Contact administrator.\n";
        return false;
    }

    if (argn > max_argn) { errorUnknownArguments(args, command_name, max_argn); return false; }

    // Check that the database has not been created, if so return.
    if(dbExists(database_name)) 
    {   
        std::cout << "!Failed to create database " << database_name << " because it already exists." << std::endl;
        return false;
    }
    try
    {
        this->databases.insert(std::make_pair(database_name, std::make_shared<Database>(database_name)));
        incrementDatabaseCount();
    }
    catch(const std::exception& e)
    {
        std::cerr << "In SQL::createDatabase => " << e.what() << '\n';
        return false;
    }
    
    return true;
}

bool SQL::dropDatabase(const std::vector<std::string>& args)
{
    const unsigned int argn = args.size();
    const unsigned int max_argn = 3;

    if (argn < max_argn) { std::cout << "[CMD - DROP - ERROR] -> Supplied argument count (" << argn << ") does not match required argument count (" << max_argn << ")\n"; return false; }

    const std::string command_name  = args[0];
    const std::string database      = args[1];
    const std::string database_name = args[2];

    // Check that the arguments are correct
    if (command_name != "DROP" || database != "DATABASE") 
    { 
        std::cout << "Programmer error in SQL.cpp -> dropDatabase. Contact administrator.\n";
        return false;
    }

    if (argn > max_argn) { errorUnknownArguments(args, command_name, max_argn); return false; }

    // Check that the database has not been created, if so return.
    if(!dbExists(database_name)) 
    {   
        std::cout << "!Failed to delete database " << database_name << " because it does not exists." << std::endl;
        return false;
    }
    try
    {
        this->databases.erase(database_name);
        decrementDatabaseCount();
    }
    catch(const std::exception& e)
    {
        std::cerr << "In SQL::dropDatabase => " << e.what() << '\n';
        return false;
    }
    
    return true;
}


bool SQL::createTable(const std::vector<std::string>& args)
{
    const unsigned int argn = args.size();

    const std::string command = args[0];
    const std::string table = args[1];
    const std::string table_name = args[2];

    if (!dbSelected())
    {
        std::cout << "!Cannot create table " << table << " because no database is selected.\n";
        return false;
    }

    return true;
}

bool SQL::dropTable(const std::vector<std::string>& args)
{

    return true;
}

std::shared_ptr<Database> SQL::useDatabase(std::shared_ptr<Database> db)
{
    if (db == nullptr) return nullptr;

    this->database = db;

    return this->database;
}

bool SQL::HANDLE_CMD(const std::vector<std::string>& args)
{
    try {
        // Grab the commands from the provided arguments
        std::string command = args[0];

        // Check if command exists, if it doesn't return 0
        if (!cmdExists(command)) {
            std::cout << "Command " << command << " does not exist.\n";
            return 0;
        }

        unsigned int command_id = cmdId(command);

        if (command_id == 0) // CREATE COMMAND HANDLER
        {
            const std::string create_type = args[1];
            if (create_type == "DATABASE") return createDatabase(args);
            else if (create_type == "TABLE") return createTable(args);
            else std::cout << create_type << " is not a valid argument of command CREATE.\n";
        }
        else if (command_id == 1) // DROP COMMAND HANDLER
        {
            const std::string drop_type = args[1];
            if (drop_type == "DATABASE") return dropDatabase(args);
            else if (drop_type == "TABLE") return dropTable(args);
            else std::cout << drop_type << " is not a valid argument of command DROP.\n";
        }
        else if (command_id == 2) // USE COMMAND HANDLER
        {
            std::cout << "USE\n";
            return 1;
        }
        else if (command_id == 3) // ALTER COMMAND HANDLER
        {
            std::cout << "ALTER\n";
            return 1;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
    }

    return 1;
}

bool SQL::dbExists(const std::string& database_name)
{
    return this->databases.count(database_name);
}

bool SQL::cmdExists(const std::string& cmd)
{
    return (bool)this->commands.count(cmd);
}

unsigned int SQL::cmdId(const std::string& cmd)
{
    return this->commands.at(cmd);
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

void SQL::errorUnknownArguments(const std::vector<std::string>& args, const std::string& cmd, unsigned int index)
{
    unsigned int argn = args.size();
    if (index > argn) return;
    std::cout << "[CMD - " << cmd << " - ERROR] -> Unknown Argument(s): {";
    for (; index < args.size(); index++) {
        std::cout << args[index];
        if (index < args.size()) std::cout << ", ";
    }
    std::cout << "}\n";
}



//std::cout << "Breakpoint\n";