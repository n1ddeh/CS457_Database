// #include "SQL.h"

// Create::Create() 
// {
//     std::cout << "Create default constructor called\n";

//     this->command = "Create"; 
// }

// Create::Create(std::vector<std::string>& arguments)
// {
//     const unsigned int argn = arguments.size();

//     if (!argn) throw "No arguments provided";
//     if (argn == 1) throw "No parameters provided";

//     initVars(arguments, argn);
//     initializeCommands();
//     this->command = "Create";

//     if (this->params_num < 2) {
//         std::cout << "[CMD] ERROR: paramter count error.\n";
//         return;
//     }

//     std::string createWhat = this->params[0];
//     if (createWhat == "DATABASE")
//     {
//         std::cout << "Create arg constructor called\n";

//         // Only two arguments should be supplied to CREATE DATABASE
//         if (this->params_num > 2) {
//             std::cout << "[CMD] ERROR: unknown argument error [";
//             for (unsigned int i = 2; i < params.size(); i++) {
//                 std::cout << this->params[i];
//                 if (i != params.size() - 1) std::cout << ", ";
//             }
//             std::cout << "]\n";
//             return;
//         }

//         // Grab the database_name
//         std::string database_name = this->params[1];        


//         // If the database already exists, return and output error message.
//         if (exists) {
//             std::cout << "!Failed to create database " << database_name << " because it already exists.\n";
//             return;
//         }
        
//         std::cout << "Database " << database_name << " created.\n";
//     }
//     else if (createWhat == "TABLE")
//     {
//         std::cout << "Creating a table\n";
//     }
//     else {
//         std::cout << "[CMD] ERROR: undefined argument " << createWhat << "\n";
//         return;
//     }

// }


// Create::~Create()
// {
//     std::cout << "Create Destructor called\n";
// }