#include "helper.h"
#include "commands/command.h"
#include "database/database.h"

class SQL
{
public:
    // Constructor
    SQL()
    {
        readUserInput();
    }

    std::vector<std::string> readUserInput()
    {
        std::string input;
        
        std::getline(std::cin, input);

        std::vector<std::string> args = helper::split(input, ' ');

        Command * cmd = new Command(args);

        readUserInput();
    }

private:
    // stores known databases
    std::unordered_map<std::string, DB::Database> databases;

};