/**
 * SQL.cpp
 * Author: Mark Minkoff
 * Desc: The source implementation of SQL.H.
 * CLI, Commands, Database Management
 */
#include "SQL.h"

SQL::SQL() : database_count(0)
{
    this->process_id = _uuid(16);
    initializeCommands();
    initializeTypes();
    readFilesystem();
    SQL_CLI();
}

SQL::SQL(std::string filePath)
{
    std::ifstream in(filePath);

    std::string str;

    while (std::getline(in, str)) {
        if (str.size() > 2 && str[0] != '-' && str[1] != '-') this->arguments.push(str);
    }

    this->process_id = _uuid(16);
    initializeCommands();
    initializeTypes();
    readFilesystem();
    SQL_CLI();
}

SQL::~SQL()
{
    fs::path p = fs::current_path();
    p += "/transactions/";
    p += this->process_id;

    if (fs::exists(p)) {
        fs::remove_all(p);
    }

    std::cout << "-- All done.\n";
}

void SQL::initializeCommands()
{
    // Specify command count and reserve memory in the unordered map for them
    std::size_t NUM_COMMANDS = 8;
    this->commands.reserve(NUM_COMMANDS);

    // Asign commands value pairs
    std::pair<std::string, unsigned int> cmd0("CREATE", 0);
    std::pair<std::string, unsigned int> cmd1("DROP",   1);
    std::pair<std::string, unsigned int> cmd2("USE",    2);
    std::pair<std::string, unsigned int> cmd3("ALTER",  3);
    std::pair<std::string, unsigned int> cmd4("SELECT", 4);
    std::pair<std::string, unsigned int> cmd5("INSERT", 5);
    std::pair<std::string, unsigned int> cmd6("UPDATE", 6);
    std::pair<std::string, unsigned int> cmd7("DELETE", 7);
    std::pair<std::string, unsigned int> cmd8("BEGIN",  8);
    std::pair<std::string, unsigned int> cmd9("CLEAR",  9);
    std::pair<std::string, unsigned int> cmdA("COMMIT", 10);

    // Insert commands into unordered map
    this->commands.insert(cmd0);
    this->commands.insert(cmd1);
    this->commands.insert(cmd2);
    this->commands.insert(cmd3);
    this->commands.insert(cmd4);
    this->commands.insert(cmd5);
    this->commands.insert(cmd6);
    this->commands.insert(cmd7);
    this->commands.insert(cmd8);
    this->commands.insert(cmd9);
    this->commands.insert(cmdA);
}

void SQL::SQL_CLI()
{
    // The user input
    std::string input;

    // Replace multiple tabs and spaces with a single space
    std::regex regx("[ \t]+");

    // File input
    if (!this->arguments.empty())
    {
        input = this->arguments.front();

        input = std::regex_replace(input, regx, std::string(" "));

        // Trim spaces from beginning and end
        input = _trim(input);

        this->arguments.pop();

        while(!this->arguments.empty() && input.back() != ';')
        {
            std::string temp = this->arguments.front();

            temp = std::regex_replace(temp, regx, std::string(" "));

            // Trim spaces from beginning and end
            temp = _trim(temp);

            input += ' ';
            input += temp;

            this->arguments.pop();
        }
        
        std::cout << input << "\n";

        if (input == ".exit") return;

        input.pop_back();
    }
    else 
    {
        // Request user input from console
        std::getline(std::cin, input);

        if (input == ".exit") return;

        while (input.back() != ';') {
            std::string temp; 
            std::getline(std::cin, temp);
            if (temp != ";") 
            {
                input += ' ';
            }
            input += temp;
        }

        input.pop_back();
    }

    // Clean up input from unecessary spaces and tabs
    input = std::regex_replace(input, regx, std::string(" "));

    // Check if the user input has balanced parenthsis, if not issue some error
    if (!_isBalancedParenthesis(input))
    {
        std::cout << "-- !Parenthsis are not balanced in input: " << input << "\n";
        return SQL_CLI();
    }

    // The exit condition for the CLI
    if (_toUpper(input) == ".EXIT" || _toUpper(input) == "EXIT")
    {
        return;
    }

    // Split the user input with a space delimeter
    std::vector<std::string> args = _split(input, ' ');

    HANDLE_CMD(args);

    return SQL_CLI();
}

bool SQL::dbSelected()
{
    // No database is selected if the database pointer is null
    if (this->database != nullptr) {
        return true;
    }
    return false;
}

bool SQL::createDatabase(const std::vector<std::string>& args)
{
    const unsigned int argn = args.size();

    // Maximum number of arguments for CREATE DATABASE command is three
    const unsigned int max_argn = 3;

    // Check if the number of argument supplied is less than the argument required
    if (argn < max_argn) { std::cout << "-- [CMD - CREATE - ERROR] -> Supplied argument count (" << argn << ") does not match required argument count (" << max_argn << ")\n"; return false; }

    const std::string command_name  = _toUpper(args[0]);
    const std::string database      = _toUpper(args[1]);
    const std::string database_name = args[2];

    // Check that the arguments are correct
    if (command_name != "CREATE" || database != "DATABASE") 
    { 
        std::cout << "-- !Programmer error in SQL::createTable. Contact admin :(\n";
        return false;
    }

    // Check if there are more supplied arguments than required
    if (argn > max_argn) { errorUnknownArguments(args, command_name, max_argn); return false; }
    
    // Check that the database has not been created, if so return.
    if(dbExists(database_name)) 
    {   
        std::cout << "-- !Failed to create database " << database_name << " because it already exists." << std::endl;
        return false;
    }
    try {
        // Generate path for database to live
        fs::path p = fs::current_path();
        p += "/storage/"; p += database_name; p += "/";

        fs::path path_metadata = p;
        path_metadata += "metadata.txt";

        // Create the database directory
        fs::create_directories(p);

        // Crate the database and insert into the unordered map of databases
        this->databases.insert(std::make_pair(database_name, std::make_shared<Database>(database_name, p, path_metadata)));

        incrementDatabaseCount();
    }
    catch(const std::exception& e)
    {
        std::cerr << " -- In SQL::createDatabase => " << e.what() << '\n';
        return false;
    }
    
    std::cout << "-- Database " << database_name << " created.\n";
    return true;
}

bool SQL::createDatabase(const std::string name, const fs::path& path, const fs::path& path_metadata)
{
    try {
        // Crate the database and insert into the unordered map of databases
        this->databases.insert(std::make_pair(name, std::make_shared<Database>(name, path, path_metadata)));

        incrementDatabaseCount();
    }
    catch(const std::exception& e)
    {
        std::cerr << " -- In SQL::createDatabase => " << e.what() << '\n';
        return false;
    }

    return true;
}

bool SQL::dropDatabase(const std::vector<std::string>& args)
{
    const unsigned int argn = args.size();
    const unsigned int max_argn = 3;

    if (argn < max_argn) { std::cout << "-- [CMD - DROP - ERROR] -> Supplied argument count (" << argn << ") does not match required argument count (" << max_argn << ")\n"; return false; }

    const std::string command_name  = _toUpper(args[0]);
    const std::string database      = _toUpper(args[1]);
    const std::string database_name = args[2];

    // Check that the arguments are correct
    if (command_name != "DROP" || database != "DATABASE") 
    {
        std::cout << "-- !Programmer error in SQL::dropTable. Contact admin :(\n";
        return false;
    }

    if (argn > max_argn) { errorUnknownArguments(args, command_name, max_argn); return false; }

    // Check that the database has not been created, if so return.
    if(!dbExists(database_name)) 
    {   
        std::cout << "-- !Failed to delete database " << database_name << " because it does not exists." << std::endl;
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
    
    std::cout << "Database " << database_name << " deleted.\n";
    return true;
}

bool SQL::createTable(const std::vector<std::string>& args)
{
    const unsigned int argn = args.size();

    const std::string command = _toUpper(args[0]);
    const std::string table = _toUpper(args[1]);
    const std::string table_name = args[2];
    std::vector<std::pair<std::string, std::string>> columns;

    if (command != "CREATE" || table != "TABLE")
    {
        std::cout << "-- !Programmer error in SQL::createTable. Contact admin :(\n";
        return false;
    }
    
    if (!dbSelected())
    {
        std::cout << "-- !Failed to create table " << table_name << " because no database is selected.\n";
        return false;
    }

    // Find first instance of character ( in table_name
    size_t found = table_name.find_first_of('(');

    // If character ( is found, rearange arguments
    if (found != std::string::npos) {
        // Get the first parameter
        std::string first_param = table_name.substr(found, std::string::npos);

        // Erase the first parameter from table_name
        const_cast<std::vector<std::string>&>(args)[2].erase(args[2].begin() + found, args[2].end());

        // Insert the first parameter after table_name in the vector
        const_cast<std::vector<std::string>&>(args).insert(args.begin() + 3, first_param);

        // Update the table name
        const_cast<std::string&>(table_name) = args[2];
    }

    if (this->database->tableExists(table_name))
    {
        std::cout << "-- !Failed to create table " << table_name << " because it already exists.\n";
        return false;
    }

    // Check if column names+types need to be parsed
    if (argn > 3)
    {
        std::vector<std::string> tableArgs(args.begin() + 3, args.end());
        columns = parseTableColumns(tableArgs);
        
        // If there are columns, preform type checking
        if (columns.size())
        {
            std::vector<std::string> types;
            for (auto& col : columns)
            {
                types.emplace_back(std::get<1>(col));
            }
            if (!checkTypes(types)) return false;
        }
    }

    this->database->createTable(table_name, columns);

    std::cout << "-- Table " << table_name << " created.\n";

    return true;
}

bool SQL::dropTable(const std::vector<std::string>& args)
{
    const unsigned int argn = args.size();

    const std::string command = _toUpper(args[0]);
    const std::string table = _toUpper(args[1]);
    const std::string table_name = args[2];

    if (command != "DROP" || table != "TABLE")
    {
        std::cout << "-- !Programmer error in SQL::dropTable. Contact admin :(\n";
        return false;
    }

    if (!dbSelected())
    {
        std::cout << "-- !Failed to drop table " << table_name << " because no database is selected.\n";
        return false;
    }

    if (!this->database->tableExists(table_name))
    {
        std::cout << "-- !Failed to drop table " << table_name << " because it does not exist.\n";
        return false;
    }

    this->database->dropTable(table_name);

    std::cout << "--Table " << table_name << " deleted.\n";

    return true;
}

bool SQL::useDatabase(std::shared_ptr<Database> db)
{
    if (db == nullptr) return false;

    this->database = db;

    std::cout << "-- Using database " << db->getDatabaseName() << ".\n";

    return true;
}

bool SQL::useDatabase(const std::vector<std::string>& args)
{
    const unsigned int argn = args.size();

    // Check for unexpected argument(s)
    if (argn > 2)
    {
        errorUnknownArguments(args, "USE", 2); // Handle unknown argument(s) error.
        return false;
    }

    if (_toUpper(args[0]) != "USE")
    {
        std::cout << "-- !Programmer error in SQL::useDatabase, contact administrator.\n";
        return false;
    }

    const std::string database_name = args[1];

    // Check if provided database_name is empty
    if (database_name.size() == 0) 
    {
        std::cout << "-- !SQL::useDatabase provided empty database_name.\n";
        return false;
    }

    // Check if the database exists - if not it's an error.
    if (!dbExists(database_name))
    {
        std::cout << "-- !Database " << database_name << " does not exist.\n";
        return false;
    }

    std::shared_ptr<Database> db = getDatabase(database_name);
    if (!db) return false;

    return useDatabase(db);
}

bool SQL::HANDLE_CMD(std::vector<std::string> args)
{
    try {
        readFilesystem();

        // Grab the commands from the provided arguments
        std::string command = _toUpper(args[0]);

        // Check if command exists, if it doesn't return 0
        if (!cmdExists(command)) {
            std::cout << "-- Command " << command << " does not exist.\n";
            return 0;
        }

        unsigned int command_id = cmdId(command);

        if (command_id == 0)     // CREATE COMMAND HANDLER
        {
            const std::string create_type = _toUpper(args[1]);
            if (create_type == "DATABASE") return createDatabase(args);
            else if (create_type == "TABLE") return createTable(args);
            else {
                std::cout << create_type << " is not a valid argument of command CREATE.\n";
                return false;
            }
        }
        else if (command_id == 1) // DROP COMMAND HANDLER
        {
            const std::string drop_type = _toUpper(args[1]);
            if (drop_type == "DATABASE") return dropDatabase(args);
            else if (drop_type == "TABLE") return dropTable(args);
            else 
            { 
                std::cout << drop_type << " is not a valid argument of command DROP.\n";
                return false;
            }
        }
        else if (command_id == 2) // USE COMMAND HANDLER
        {
            return useDatabase(args);
        }
        else if (command_id == 3) // ALTER COMMAND HANDLER
        {
            return alterTable(args);
        }
        else if (command_id == 4) // SELECT COMMAND HANDLER
        {
            return selectTable(args);
        }
        else if (command_id == 5) // INSERT COMMAND HANDLER
        {
            const std::string insert_type = _toUpper(args[1]);
            if (insert_type == "INTO") return insertInto(args);
            else {
                std::cout << "-- Invalid insert specifier: " << insert_type << "\n";
                return false;
            }
        }
        else if (command_id == 6) // UPDATE COMMAND HANDLER
        {
            return updateTable(args);
        }
        else if (command_id == 7) // DELETE COMMAND HANDLER
        {
            return deleteFromTable(args);
        }
        else if (command_id == 8) // TRANSACTION COMMAND HANDLER
        {
            return beginTransaction(args);
        }
        else if (command_id == 9)
        {
            system("clear");
            return true;
        }
        else if (command_id == 10)
        {
            return commit(args);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
    }

    return true;
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

void SQL::errorUnknownArguments(const std::vector<std::string>& args, const std::string& cmd, unsigned int index)
{
    unsigned int argn = args.size();
    if (index > argn) return;
    std::cout << "-- [CMD-" << cmd << " - ERROR] -> Unknown Argument(s): {";
    for (; index < args.size(); index++) {
        std::cout << args[index];
        if (index < args.size()-1) std::cout << ", ";
    }
    std::cout << "}\n";
}

std::shared_ptr<Database> SQL::getDatabase(const std::string& database_name)
{
    if (!dbExists(database_name))
    {
        return nullptr;
    }

    return this->databases.at(database_name);
}

std::vector<std::pair<std::string, std::string>> SQL::parseTableColumns(std::vector<std::string> columns)
{
    unsigned int num_columns = columns.size();

    // Check if column arguments were provided.
    if (!num_columns)
    {
        // If not, return an empty column list
        std::cout << "-- !column arguments for CREATE TABLE do not exist\n";
        return std::vector<std::pair<std::string, std::string>>();
    }

    // Check if only two column arguments were provided
    if (num_columns == 2)
    {
        std::vector<std::pair<std::string, std::string>> res;
        std::string tempName = columns[0];
        std::string tempType = columns[1];
        unsigned int tempTypeSize = tempType.size();
        if (tempName[0] == '(') {
            columns[0].erase(columns[0].begin());
        }
        if (tempType.back() == ')' && (tempType[tempTypeSize - 2] == ')') && (!isdigit(tempType[tempTypeSize - 2])))
        {
            tempType.pop_back(); // maybe
        }

        res.emplace_back(std::make_pair(tempName, tempType));
        return res;
    }

    if ((num_columns % 2) != 0)
    {
        std::cout << "-- !Number of column arguments for CREATE TABLE are not even\n";
        // This means that the user inputed a missing name/type for column arguments
        return std::vector<std::pair<std::string, std::string>>();
    }

    // Check that the column arguments start and end with paranthesis
    if (! (columns[0][0] == '(') || ! (columns[num_columns-1].back() == ')'))
    {
        std::cout << "-- !Column arguments for CREATE TABLE are not wrapped with ()\n";
        return std::vector<std::pair<std::string, std::string>>();
    }
    
    // Delete the wrapped parenthesis
    columns[num_columns-1].pop_back();      // Last character of last string
    columns[0].erase(columns[0].begin());   // First character of first string

    std::string tempName;
    std::string tempType;
    std::vector<std::pair<std::string, std::string>> res;
    for (unsigned int index = 0; index < num_columns; ++index)
    {
        if (tempName.size() && tempType.size())
        {
            res.emplace_back(std::make_pair(tempName, tempType));
            tempName.clear(); tempType.clear();
        }

        if (index % 2) // 1, 3, 5, ...
        {
            tempType = columns[index];
            if (index < num_columns-1 && tempType.back() != ',')
            {
                std::cout << "-- !CREATE table error: Missing ',' after datatype " << tempType << ".\n";
                return std::vector<std::pair<std::string, std::string>>();
            }
            else if(columns.back() != tempType){
                tempType.pop_back();
            }
        }
        else // 0, 2, 4, 6
        {
            tempName = columns[index];
        }
    }
    if (tempName.size() && tempType.size())
    {
        res.emplace_back(std::make_pair(tempName, tempType));
    }

    return res;
}

bool SQL::selectTable(const std::vector<std::string>& args)
{
    const unsigned int argn = args.size();

    if (argn < 4) { std::cout << "-- !Invalid number of arguments for command SELECT\n"; return 0; }

    const std::string command = _toUpper(args[0]);
    if (command != "SELECT") { std::cout << "-- !Programmer error in SQL::selectTable.\n";  return false; }

    std::string table_name;

    if (args[1] == "*")
    {
        std::string from = _toUpper(args[2]);
        if (from != "FROM") { std::cout << "-- !Unknown argument from command SELECT *: " << from << ". Did you mean FROM?\n"; return false; }

        if (argn == 4) {
            table_name = args[3];

            // Handle the SELECT * FROM {{ table_name }}; command
            return this->selectAllFromTable(table_name);
        }
        
        return this->selectAllQuery(args);
    }
    
    std::vector<std::string> columns_to_search;

    size_t index = 1;
    while (index < argn - 1 && _toUpper(args[index]) != "FROM")
    {
        columns_to_search.emplace_back(args[index]);
        if (columns_to_search[index - 1].back() == ',')  {
            columns_to_search[index - 1].pop_back();
        }
        ++index;
    }

    if (columns_to_search.empty()) { std::cout << "-- !Failed to query any tables. Did you for get the add column names after the SELECT statement?\n"; return false; }

    const std::string from = _toUpper(args[index++]);
    if (from != "FROM") { std::cout << "-- Unknown command " << from << ". Did you mean FROM?\n"; return false; }

    table_name = args[index++];
    // If the table does NOT exist, alert the user and return false
    if (!this->database->tableExists(table_name)) { std::cout << "-- !Failed to update table " << table_name << " because it does not exist.\n"; return false; }

    const std::string where = _toUpper(args[index++]);
    if (where != "WHERE") { std::cout << "--!Failed to query table " << table_name << ". Unknown argument " << where << ". Did you mean 'WHERE'?\n"; return false; }

    const std::string column_to_query = args[index++];
    const std::string opr = args[index++];

    // If this is NOT a valid operator, return false
    if (!_isValidOperator(opr)) { std::cout << "-- !Failed to query tables because the operator " << opr << " is not supported. Did you mean '='?\n"; return false; }

    const std::string value_to_query = args[index];

    std::shared_ptr<Table> table = this->database->getTable(table_name);

    return table->selectColumns(columns_to_search, column_to_query, value_to_query, opr);
}

bool SQL::selectAllQuery(const std::vector<std::string>& args)
{
    const std::string select = _toUpper(args[0]);
    const std::string all = _toUpper(args[1]);
    const std::string from = _toUpper(args[2]);

    // Ensure we are handeling the correct arguments
    if (select != "SELECT" || all != "*" || from != "FROM") {
        std::cout << "-- !Programmer error in SQL::selectAllQuery.\n";  return false;
    }

    // Parse all words between 'from' and ('on' or 'where')
    std::vector<std::string> table_clause;
    size_t index = 3;
    while (index < args.size() && (_toUpper(args[index]) != "ON" && _toUpper(args[index]) != "WHERE")) {
        table_clause.emplace_back(args[index++]);
    }

    if (table_clause.size() < 4 || table_clause.size() == 5) { std::cout << "== !Incorrect table clause for SELECT * FROM\n"; return false; }

    if (table_clause[1].back() == ',') table_clause[1].pop_back();

    // Initilize table_name table_nickname containers
    std::pair<std::string, std::string> table_1 = std::make_pair(table_clause[0], table_clause[1]);
    std::pair<std::string, std::string> table_2 = std::make_pair(table_clause[table_clause.size() - 2], table_clause[table_clause.size() - 1]);

    // Set the selection parameters
    short int join = 0;
    bool left = true, right = false, inner = true;

    index = 2;
    if (_toUpper(table_clause[index]) == "RIGHT") { left = false; right = true; }
    else if (_toUpper(table_clause[index] == "FULL")) { right = true; }
    if (_toUpper(table_clause[index]) == "OUTER" || _toUpper(table_clause[index + 1]) == "OUTER") { inner = false; }

    // Get the index of the 'WHERE' or 'ON' keyword
    index = 0;
    while (index < args.size() && _toUpper(args[index]) != "ON" && _toUpper(args[index]) != "WHERE") ++index;
    if (index >= args.size() - 1) { std::cout << "-- !Query failed. Missing 'WHERE' or 'ON' token\n"; return false; }

    // Collect the query statement
    std::vector<std::string> query_statement;
    while (index < args.size()) query_statement.emplace_back(args[index++]);

    return this->database->queryTables(
        table_1, 
        table_2,
        std::make_pair(left, right),
        inner,
        query_statement
    );
}

bool SQL::selectAllFromTable(const std::string& table_name)
{
    try {
        std::shared_ptr<Table> table = this->database->getTable(table_name);

        readCSV(table, table->getPath());

        return table->printAll();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
    }
    
    return false;
}

bool SQL::alterTable(const std::vector<std::string>& args)
{
    const unsigned int argn = args.size();
    const std::string command = args[0];
    const std::string table = args[1];
    const std::string table_name = args[2];
    const std::string alter_type = args[3];

    std::vector<std::string> tableArgs(args.begin() + 4, args.end());
    std::vector<std::pair<std::string, std::string>> columns = parseTableColumns(tableArgs);

    if (command != "ALTER" || table != "TABLE")
    {
        std::cout << "-- !Programmer error in SQL::alterTable. Contact admin :(\n";
        return false;
    }

    if (!database->tableExists(table_name))
    {
        std::cout << "-- !Could not modify table " << table_name << " because it did not exist.\n";
        return false;
    }

    this->database->addColumnsToTable(table_name, columns);

    std::cout << "-- Table " << table_name << " modified.\n";
    return true;
}

void SQL::initializeTypes()
{
    // Specify type count and reserve memory in the unordered map for them
    std::size_t NUM_TYPES = 4;
    this->types.reserve(NUM_TYPES);

    std::pair<std::string, unsigned int> type0("INT", 0);
    std::pair<std::string, unsigned int> type1("FLOAT", 1);
    std::pair<std::string, unsigned int> type2("CHAR", 2);
    std::pair<std::string, unsigned int> type3("VARCHAR", 3);

    // Insert commands into unordered map
    this->types.insert(type0);
    this->types.insert(type1);
    this->types.insert(type2); 
    this->types.insert(type3);
}

/*  From a list of types, check if any are invalid
    Print all invalid types 
    Return true if there are no errors
    Return false if there are errors */
bool SQL::checkTypes(std::vector<std::string> types)
{
    // Initialize errors container
    std::vector<std::string> errors;

    for (auto type: types)
    {
        std::string upperType = _toUpper(type);
        if (!this->types.count(upperType))
        {
            // Check if this is a VARCHAR and parathensis are formatted correctly
            if (upperType.substr(0, 8) == "VARCHAR(" && upperType.back() == ')')
            {
                // Comfirmed VARCHAR, now ensure the number is valid
                std::string num = upperType.substr(8, upperType.size() - 9);
                
                // Check if user entered a negative number
                if (num[0] == '-') 
                {
                    std::string temp = "-- TYPE ERORR: VARCHAR size (";
                    temp += num;
                    temp += ") cannot be negative.\n";
                    errors.emplace_back(temp);
                }
                // Check if any character is not valid for varchar size
                for (auto& c: num)
                {
                    if (!isdigit(c)) 
                    {
                        std::string temp = "-- TYPE ERROR: VARCHAR size (";
                        temp += num;
                        temp += ") contains non digit characters.\n";
                        errors.emplace_back(temp);
                    }
                }
            }
            else {
                std::string temp = "-- TYPE ERORR: Unknown type: ";
                temp += type;
                temp += '\n';
                errors.emplace_back(temp);
            }
        }
    }
    // If there are type errors, print them out and return false
    if (errors.size()) {
        for (auto& e: errors) {
            std::cout << e;
        }
        return false;
    }
    // Return true when there are no errors
    return true;
}

bool SQL::insertInto(const std::vector<std::string>& args)
{
    const std::string command = _toUpper(args[0]);
    const std::string command_type = _toUpper(args[1]);

    // Make sure we are handling the correct command
    if (command != "INSERT" && command_type != "INTO")
    {
        std::cout << "-- Programmer error in insertInto :(\n";
        return 0;
    }

    // Ensure a database is selected
    if (!dbSelected())
    {
        std::cout << "-- Database not selected\n";
        return false;
    }

    const std::string table_name = args[2];

    // Ensure table exists in the selected database
    const bool table_exists = this->database->tableExists(table_name);
    if (!table_exists)
    {
        std::cout << "-- Table " << table_name << " does not exist in database " << database->getDatabaseName() << "\n";
        return false;
    }

    // Create a sub vector of just the parameters
    std::vector<std::string> paramsVec = _subVector(args, args.begin() + 3, args.end());

    // Join the parameters as a string with space as delimeter
    std::string paramsString = _join(paramsVec, std::string(" "));

    // Replace multiple tabs and spaces with a single space
    std::regex regx("[ \t]+");
    paramsString = std::regex_replace(paramsString, regx, std::string(""));

    // Ensure the user specified correct formatting
    std::string values = _toUpper(std::string(paramsString.begin(), paramsString.begin() + 7));
    if (values != "VALUES(" || paramsString.back() != ')') 
    {
        std::cout << "-- INSERT INTO parameters not formatted correctly. Correct format is VALUES(x, y, z, ...)\n";
        return false;
    }

    // Parse the parameters we want to insert: parse format =  x,y,z,...
    std::string paramsParsed = std::string(paramsString.begin() + 7, paramsString.end() - 1);

    // Isolate the parameters without their delimeters
    std::vector<std::string> isolatedParams = isolateParams(paramsParsed);

    // Get a pointer to the table we want to insert into
    std::shared_ptr<Table> table = this->database->getTable(table_name);

    // Return the insertRow function in table
    bool success = table->insertRow(isolatedParams, true);

    if (!success) return false;

    std::cout << "-- 1 new record inserted.\n"; 

    return true;
}

std::vector<std::string> SQL::isolateParams(std::string params, char delim)
{
    // Initialize an empty vector container of strings for each parameter in params
    std::vector<std::string> container((size_t)1, std::string(""));

    // Initialize index to track each position of the container to insert characters into
    size_t index = 0;

    // Initialize stack to track if we are parsing quotes and therefore a varchar
    std::stack<char> quotes;

    // Iterate over every character of params, and parse each parameter
    for (auto& c : params) 
    {
        // The comma is a delimeter, so it marks the start of the next parameter unless we are parsing a varchar
        if (c == delim && quotes.empty()) 
        {
            ++index;
            container.emplace_back(std::string(""));
        }
        // Parsing the character " so insert into stack if applicable 
        else if (c == 34) 
        { 
            if (!quotes.empty() && quotes.top() == 34) quotes.pop();
            else if (quotes.empty() && (container[index].empty())) quotes.push(34);
            else container[index] += c;
        }
        // Parsing the character ' so insert into stack if applicable 
        else if (c == 39) 
        {
            if (!quotes.empty() && quotes.top() == 39) quotes.pop();
            else if (quotes.empty() && (container[index].empty())) quotes.push(39); 
            else container[index] += c;
        }
        // Valid character is appended into the container at index
        else {
            container[index] += c;
        }
    }

    return container;
}

bool SQL::updateTable(const std::vector<std::string>& args)
{
    // Grab the command
    const std::string command = _toUpper(args[0]);

    // The function handles the UPDATE {{ table_name }} SET .. command.
    if (command != "UPDATE") { std::cout << "-- Programmer error in updateTable\n"; return false; }

    // If the database has NOT been selected, alert the user and return false.
    if (!dbSelected()) { std::cout << "-- Database not selected\n"; return false; }

    // Check metadata for any updates
    const DatabaseMetadata db_md = this->readDatabaseMetadata(this->database->getPathMetadata());
    this->database->applyMetadata(db_md);

    // Grab the table name
    const std::string table_name = args[1];

    // If the table does NOT exist, alert the user and return false
    if (!this->database->tableExists(table_name)) { std::cout << "-- !Failed to update table " << table_name << " because it does not exist.\n"; return false; }

    // Grab the SET keyword
    const std::string set = _toUpper(args[2]);

    // Only the SET keyword is allowed for the UPDATE {{ table_name }} command.
    // If the argument is not SET alert the user and return false
    if (set != "SET") { std::cout << "-- Unknown command " << set << ". Did you mean SET?\n"; return false; }

    // Get the name of the column we want to update
    const std::string column_to_update = args[3];

    // Fetch the table ptr
    std::shared_ptr<Table> table = this->database->getTable(table_name);

    // Get the first operator, this should always be '='
    const std::string op1 = args[4];
    
    // If the operator is NOT '=' alert the user and return false.
    if (op1 != "=") { std::cout << "-- !Failed to update table " << table_name << " because the first operator " << op1 << " is not supported. Did you mean '='?\n"; return false; }

    // Get the value we want to update
    std::string value_to_update = args[5];

    // If the first and last characer are single or doubles quotes, erase and the first and last character
    if (value_to_update.size() > 2 && 
        ((value_to_update[0] == 39 && value_to_update.back() == 39) || (value_to_update[0] == 34 && value_to_update.back() == 34))
    ) 
    {
        value_to_update = value_to_update.substr(1, value_to_update.size() - 2);
    }

    // Get the query parameter 'WHERE', we expect this to always be included
    const std::string where = _toUpper(args[6]);

    if (where != "WHERE") { std::cout << "--!Failed to update table " << table_name << ". Unknown argument " << where << ". Did you mean 'WHERE'?\n"; return false; }

    // Get the name of the column we want to search
    const std::string column_to_search = args[7];

    // Get the second operator
    const std::string op2 = args[8];

    // If this is NOT a valid operator, return false
    if (!_isValidOperator(op2)) { std::cout << "-- !Failed to update table " << table_name << " because the second operator " << op2 << " is not supported. Did you mean '='?\n"; return false; }

    // Get the value we want to search for
    std::string value_to_search = args[9];

    // If the first and last characer are single or doubles quotes, erase and the first and last character
    if (value_to_search.size() > 2 && 
        ((value_to_search[0] == 39 && value_to_search.back() == 39) || (value_to_search[0] == 34 && value_to_search.back() == 34))
    ) 
    {
        value_to_search = value_to_search.substr(1, value_to_search.size() - 2);
    }

    const TableMetadata t_md = this->readTableMetadata(table->getPathMetadata());
    table->applyMetadata(t_md);

    bool mode = true;
    if (this->database->getTransaction() == true)
    {
        if (_toUpper(table->getLocked()) != "FALSE" && table->getLocked() != "0" && table->getLocked() != this->process_id) {
            std::cout << "-- Error: Table " << table_name << " is locked!\n";
            return false;
        }
        
        else if (table->getLocked() == this->process_id || _toUpper(table->getLocked()) == "FALSE" || table->getLocked() == "0" ) {
            if (_toUpper(table->getLocked()) != this->process_id) {
                table->setLocked(this->process_id);
                table->writeMetadata();
            }
            fs::path p = fs::current_path();
            p += "/transactions/"; p += this->process_id; p += "/";
            if (!fs::exists(p)) {
                fs::create_directories(p);
            }

            p += table_name; p += ".txt";
            std::ofstream transaction_file(p, std::ofstream::out | std::ofstream::ate );
            
            std::string command = _join(args, " ");
            command += ";";

            transaction_file << command << "\n";

            if (transaction_file.is_open()) {
                transaction_file.close();
            }

            mode = false;
        }
    }

    // Query the table to update based on these parameters
    bool success = table->updateColumnSet(column_to_update, column_to_search, value_to_update, value_to_search, op2, mode);
    table->writeMetadata();
    table->writeCSV();

    return success;
}

bool SQL::deleteFromTable(const std::vector<std::string>& args)
{
    // Get the command from the arguments
    const std::string command = _toUpper(args[0]);

    // The function handles the DELETE {{ table_name }} FROM .. command. 
    if (command != "DELETE") { std::cout << "-- Programmer error in deleteFromTable\n"; return false; }

    // Get the 'FROM' keyword
    const std::string from = _toUpper(args[1]);
    
    // Only the FROM keyword is allowed for the DELETE FROM {{ table_name }} command.
    // If the argument is not FROM alert the user and return false
    if (from != "FROM") { std::cout << "-- Unknown command " << from << ". Did you mean FROM?\n"; return false; }

    // Grab the table name
    const std::string table_name = args[2];
    
    // If the table does NOT exist, alert the user and return false
    if (!this->database->tableExists(table_name)) { std::cout << "-- !Failed to update table " << table_name << " because it does not exist.\n"; return false; }

    // Get the 'WHERE' keyword
    const std::string where = _toUpper(args[3]);
    if (where != "WHERE") { std::cout << "-- Unknown command " << where << ". Did you mean WHERE?\n"; return false; }

    // Grab the name of the column we want to search
    const std::string column_to_search = args[4];

    // Grab the operator we want to preform the search on
    const std::string opr = args[5];

    // If this is NOT a valid operator, return false
    if (!_isValidOperator(opr)) { std::cout << "-- !Failed to delete from table " << table_name << " because the second operator " << opr << " is not supported. Did you mean '='?\n"; return false; }

    // Grab the value we want to search for
    std::string value_to_search = args[6];

    // If the first and last characer are single or doubles quotes, erase and the first and last character
    if (value_to_search.size() > 2 && 
        ((value_to_search[0] == 39 && value_to_search.back() == 39) || (value_to_search[0] == 34 && value_to_search.back() == 34))
    ) 
    {
        value_to_search = value_to_search.substr(1, value_to_search.size() - 2);
    }

    // Fetch the table ptr
    std::shared_ptr<Table> table = this->database->getTable(table_name);

    return table->deleteFromTable(column_to_search, value_to_search, opr);
}

bool SQL::beginTransaction(const std::vector<std::string>& args)
{
    unsigned int n = args.size();
    const std::string begin = _toUpper(args[0]);
    const std::string transaction = _toUpper(args[1]);

    if (begin != "BEGIN" || transaction != "TRANSACTION") {
        std::cout << "Programmer error in beginTransaction - returning\n";
        return false;
    }

    if (n > 2) {
        errorUnknownArguments(args, "BEGIN TRANSACTION", 2);
        return false;
    }

    if (!this->dbSelected()) {
        std::cout << "-- Database not selected\n";
        return false;
    }

    if (this->database->getTransaction() == true) {
        std::cout << "-- Transaction already occuring in " << this->database->getDatabaseName() << "\n";
    }
    else { 
        this->database->setTransaction(true);
        fs::path p = fs::current_path();
        p += "/transactions/"; p += this->process_id; p += "/";
        fs::create_directories(p);
        std::cout << "-- Transaction starts.\n";
        this->database->writeMetadata();
    }

    return this->database->getTransaction(); 
}

bool SQL::readFilesystem()
{
    // Initialize path for filesystem storage directory
    fs::path storage_path = fs::current_path();
    storage_path += "/storage/";

    // If the storage directory does NOT exist, create it and return.
    if (!fs::exists(storage_path)) {
        fs::create_directories(storage_path);
        return false;
    }

    // Iterate over every directory in storage directory
    for (const auto& path : fs::directory_iterator(storage_path)) {
        // Ensure that the current path is a directory
        if (fs::is_directory(path)) 
        {
            // Get the name of the database from the name of the file directory
            std::string db_name = (std::string)fs::path(path).filename();

            // Get the path of the database metadata file
            fs::path metadata_path = path; metadata_path += "/metadata.txt";

            // If the metadata file exists, parse it
            if (fs::exists(metadata_path))
            {
                const DatabaseMetadata db_md = this->readDatabaseMetadata(metadata_path);
                
                // Create the daatabase and if successful, continue
                if (createDatabase(db_name, path, metadata_path)) {

                    // Get a pointer to the database and set the transaction mode
                    auto db = this->getDatabase(db_name);
                    db->setDatabaseName(db_md.database_name);
                    db->setTransaction(db->getTransaction());

                    // Iterate over every directory in the database directory
                    for (const auto& table_path : fs::directory_iterator(path)) 
                    {
                        if (fs::is_directory(table_path))
                        {
                            // Get the name of the table from the directory name
                            std::string table_name = (std::string)fs::path(table_path).filename();

                            // Get the path of the table metadata file
                            fs::path md_path = table_path; md_path += "/"; md_path += table_name; md_path += ".txt";

                            const TableMetadata table_metadata = this->readTableMetadata(md_path);

                            if (db->tableExists(table_metadata.table_name))
                            {
                                auto table = db->getTable(table_metadata.table_name);
                                table->setLocked(table_metadata.locked);
                                fs::path csv_path = table_path; csv_path += "/"; csv_path += table_name; csv_path += ".csv";
                            }

                            // Create the table, and if successful, continue
                            else if (db->createTable(table_metadata.table_name, table_metadata.column_meta_data)) 
                            {
                                // Get a pointer to the table and set the locked state
                                auto table = db->getTable(table_metadata.table_name);
                                table->setLocked(table_metadata.locked);

                                // Get the path of the csv file
                                fs::path csv_path = table_path; csv_path += "/"; csv_path += table_name; csv_path += ".csv";

                                this->readCSV(table, csv_path);
                            }
                        }
                    }
                }
            }              
        }
        else if (fs::is_regular_file(path)) continue;
    }

    return true;
}

const DatabaseMetadata SQL::readDatabaseMetadata(const fs::path& path)
{
    if (fs::exists(path))
    {
        // Open an input stream
        std::ifstream metadata_file(path, std::ifstream::in);

        // Initialize variables we are checking for in metadata file
        std::string db_name;
        fs::path db_path, md_path;
        bool transaction_mode;

        // Iterate over every line in metadata file, look for specific attributes, and parse those attributes
        std::string line; int index;
        while (std::getline(metadata_file, line))
        {
            if ((index = line.find("database_name: ", 0)) != std::string::npos) {
                db_name = line.substr(index + sizeof("database_name") + 1, line.length());
            }
            else if ((index = line.find("database_path: ", 0)) != std::string::npos) {
                db_path = line.substr(index + sizeof("database_path") + 1, line.length());
            } 
            else if ((index = line.find("metadata_path: ", 0)) != std::string::npos) {
                md_path = line.substr(index + sizeof("metadata_path") + 1, line.length());
            }
        }

        DatabaseMetadata md(db_name, db_path, md_path);
        
        return md;
    }

    DatabaseMetadata err("undefined", "undefined", "undefined");

    return err;
}

const TableMetadata SQL::readTableMetadata(const fs::path& path)
{
    // If the metadata file exists for the table, parse it
    if (fs::exists(path))
    {
        // Open an input stream
        std::ifstream metadata_file(path, std::ifstream::in);

        // Initialize varaibles we want to collect about the table.
        std::string table_name, columns;
        fs::path table_path, metadata_path;
        std::vector<std::pair<std::string, std::string>> column_meta_data;
        unsigned int column_count = 0, row_count = 0;
        std::string locked = "false";

        // Iterate over every line in the metadata file
        size_t index;
        std::string line;
        while (std::getline(metadata_file, line))
        {
            if ((index = line.find("table_name: ", 0)) != std::string::npos) {
                table_name = line.substr(index + sizeof("table_name") + 1, line.length());
            } 
            else if ((index = line.find("columns: ", 0)) != std::string::npos) {
                columns = line.substr(index + sizeof("columns") + 1, line.length());
                std::vector<std::string> split_columns = _split(columns, ',');

                for (auto& c : split_columns) {
                    std::vector<std::string> col = _split(c, ' ');
                    column_meta_data.emplace_back(std::make_pair(col[0], col[1]));
                }
            }
            else if ((index = line.find("column_count: ", 0)) != std::string::npos) {
                column_count = std::stoi(line.substr(index + sizeof("column_count") + 1, line.length()));
            } 
            else if ((index = line.find("row_count: ", 0)) != std::string::npos) {
                row_count = std::stoi(line.substr(index + sizeof("row_count") + 1, line.length()));
            }
            else if ((index = line.find("table_path: ", 0)) != std::string::npos) {
                table_path = line.substr(index + sizeof("table_path") + 1, line.length());
            }
            else if ((index = line.find("metadata_path: ", 0)) != std::string::npos) {
                metadata_path = line.substr(index + sizeof("metadata_path") + 1, line.length());
            }
            else if ((index = line.find("locked: ", 0)) != std::string::npos) {
                locked = line.substr(index + sizeof("locked") + 1, line.length());
            }
        }

        // If the file is still open, close it
        if (metadata_file.is_open()) metadata_file.close();

        const TableMetadata metadata(
            table_name,
            column_meta_data,
            column_count,
            row_count,
            table_path,
            metadata_path,
            locked
        );

        return metadata;
    }

    return TableMetadata();
}

bool SQL::readCSV(std::shared_ptr<Table> table, const fs::path& path)
{
    // If the csv file exists and is NOT a directory, continue
    if (fs::exists(path) && fs::is_regular_file(path))
    {

        unsigned int row_count = table->getRowCount();

        for(size_t i = 0; i < row_count; i++) {
            table->deleteRow(0);
        }

        // Open an input stream
        std::ifstream csv_file(path, std::ifstream::in);

        // Initialize a string container
        std::string line;

        // Do nothing with the first line (the first line are headers)
        std::getline(csv_file, line);

        std::vector<std::vector<std::string>> rows;

        // Iterate over every line in the csv
        while (std::getline(csv_file, line)) 
        {
            // Split the line by commas, spltting columns by a vector of (name, type)
            rows.emplace_back( _split(line, ','));
        }

        for (auto& row : rows) {
            // Insert the row into the table
            try {
                table->insertRow(row, false);
            }
            catch(const std::exception& e) {
                // Do nothing with exception and continue
            }
        }
        // True for success
        return true;                                
    }
    // False for failure
    return false;
}

bool SQL::commit(const std::vector<std::string>& args)
{
    unsigned int n = args.size();
    const std::string commit = _toUpper(args[0]);

    if (commit != "COMMIT") {
        std::cout << "Programmer error in commit - returning\n";
        return false;
    }

    if (n > 1) {
        errorUnknownArguments(args, "COMMIT", 1);
        return false;
    }

    if (!this->dbSelected()) {
        std::cout << "-- Database not selected\n";
        return false;
    }

    if (!this->database->getTransaction()) {
        std::cout << "-- Transaction has not begun.\n";
        return false;
    }

    fs::path p = fs::current_path();
    p += "/transactions/";
    p += this->process_id;

    if (!fs::exists(p))
    {
        std::cout << "-- No commits to be made.\n";
        return false;
    }

    this->database->setTransaction(false);
    for (auto& path : fs::directory_iterator(p))
    {
        std::string table_name = fs::path(path).filename();
        table_name = table_name.substr(0, table_name.size() - 4);

        if (this->database->tableExists(table_name))
        {
            std::shared_ptr<Table> table = this->database->getTable(table_name);

            fs::path file_path = path;

            std::ifstream command_file(file_path, std::ifstream::in);

            std::string line;
            while (std::getline(command_file, line)) {
                std::vector<std::string> args = _split(line, ' ');
                if (!args.empty()) {
                    HANDLE_CMD(args);
                }   
            }

            if (command_file.is_open()) {
                command_file.close();
            }

            fs::remove_all(file_path);
            table->setLocked("false");
            table->writeMetadata();
        }
    }

    fs::remove_all(p);
    std::cout << "Transaction commited.\n";

    return true;
}
