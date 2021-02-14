#include "SQL.h"

SQL::SQL() 
{
    SQL::readUserInput();

    this->db_selected = false;
    this->database = nullptr;
}

void SQL::readUserInput()
{
    std::string input;
    
    std::getline(std::cin, input);

    std::vector<std::string> args = SQL::split(input, ' ');

    CMD * cmd = new CMD(args);

    readUserInput();
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