#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>

class Command
{
protected:
    std::string              command;
    std::vector<std::string> params;
    unsigned int             params_num;
    std::string              database;
    std::string              database_loc;
    std::string              table;
    std::string              table_loc;

private:
    std::unordered_map<std::string, unsigned int> commands;


    
public:
    Command(std::vector<std::string>& arguments)
    {
        const unsigned int argn = arguments.size();

        if (!argn) throw "No arguments provided";

        if (argn == 1) throw "No parameters provided";

        initVars(arguments, argn);

        initializeCommands();

        handle();
    }

    ~Command()
    {

    }

private:
    /* Local protected variables
       are initialized */
    void initVars(std::vector<std::string>& args, const unsigned int argn)
    {
        // First argument is always the commands
        this->command = args[0];

        // Store the parameters in a list
        this->params.reserve(argn - 1);
        std::move(args.begin() + 1, args.end(), std::back_inserter(this->params));
        this->params_num = argn - 1;

        // We don't have these yet
        this->database      = "undefined";
        this->database_loc  = "undefined";
        this->table         = "undefined";
        this->table_loc     = "undefined";
    }

    /*  Existing commands are 
        mapped and initialized */
    void initializeCommands()
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

    void handle()
    {
        try
        {

            if (!this->commands.count(this->command)) {
                std::cout << this->command << " is not a command.\n";
            }
            
            unsigned int cmd = this->commands.at(this->command);

            switch (cmd)
            {
                case 0:
                    std::cout << "CREATE COMMAND HIT";
                    break;

                case 1:
                    std::cout << "DROP COMMAND HIT";
                    break;

                case 2:
                    std::cout << "USE COMMAND HIT";
                    break;

                case 3:
                    std::cout << "ALTER COMMAND HIT";
                    break;
                    
                default:
                    std::cout << "UNDEFINED COMMAND" << std::endl;
                    break;
            }

        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << "\n";
        }
        
    }
};