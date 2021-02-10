#include "helper.h"
#include "commands/command.h"

class kernal
{
public:
    // Constructor
    kernal()
    {
        readUserInput();
    }

    std::vector<std::string> readUserInput()
    {
        std::string input;
        
        std::getline(std::cin, input);

        std::vector<std::string> args = helper::split(input, ' ');

        Command * cmd = new Command(args);

        return args;
    }
};