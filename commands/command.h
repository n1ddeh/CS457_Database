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
        this->command = args[0];
        this->params.reserve(argn - 1);
        std::move(args.begin() + 1, args.end(), std::back_inserter(this->params));
        this->params_num = argn - 1;
        this->database = "undefined";
        this->database_loc = "undefined";
        this->table = "undefined";
        this->table_loc = "undefined";
    }

    /*  Existing commands are 
        mapped and initialized */
    void initializeCommands()
    {
        std::pair<std::string, unsigned int> cmd1("CREATE", 0);
        std::pair<std::string, unsigned int> cmd2("DELETE", 1);

        this->commands.insert(cmd1);
        this->commands.insert(cmd2);        
    }

    void handle()
    {
        try
        {
            unsigned int val = this->commands.at(this->command);

            switch (val)
            {
            case 0:
                std::cout << "CREATE COMMAND HIT" << std::endl;
                break;
            case 1:
                std::cout << "DELETE COMMAND HIT" << std::endl;
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