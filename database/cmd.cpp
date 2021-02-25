// #include "SQL.h"

// CMD::CMD() {
//     initializeCommands();
// }

// CMD::~CMD() {}

// // Constructor
// CMD::CMD(std::vector<std::string>& arguments)
// {
//     const unsigned int argn = arguments.size();
//     if (!argn) throw "No arguments provided";
//     if (argn == 1) throw "No parameters provided";

//     initVars(arguments, argn);
//     initializeCommands();
// }

// void CMD::initVars(std::vector<std::string>& args, const unsigned int argn)
// {
//     // First argument is always the commands
//     this->command = args[0];

//     // Store the parameters in a list
//     this->params.reserve(argn - 1);
//     std::move(args.begin() + 1, args.end(), std::back_inserter(this->params));
//     this->params_num = argn - 1;

//     // We don't have these yet
//     this->database = nullptr;
//     this->database_name = "undefined";
//     this->database_path = "undefined";
//     this->table = nullptr;
//     this->table_name = "undefined";
//     this->table_path = "undefined";
// }

// void CMD::initializeCommands() {
//     std::size_t NUM_COMMANDS = 4;

//     this->commands.reserve(NUM_COMMANDS);

//     std::pair<std::string, unsigned int> cmd1("CREATE", 0);
//     std::pair<std::string, unsigned int> cmd2("DROP",   1);
//     std::pair<std::string, unsigned int> cmd3("USE",    2);
//     std::pair<std::string, unsigned int> cmd4("ALTER",  3);

//     this->commands.insert(cmd1);
//     this->commands.insert(cmd2);
//     this->commands.insert(cmd3); 
//     this->commands.insert(cmd4); 
// }

// bool CMD::checkCMD(const std::string& command) {
//     return this->commands.count(command);
// }

// unsigned int CMD::get_CMD_ID(const std::string& command) {
//     return this->commands.at(command);
// }