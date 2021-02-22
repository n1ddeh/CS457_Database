#ifndef CMD_H_
#define CMD_H_

#include "../database/SQL.h"


class CMD
{
protected:
    std::string              command;
    std::vector<std::string> params;
    unsigned int             params_num;
    Database*                database;
    std::string              database_name;
    std::string              database_path;
    Table*                   table;
    std::string              table_name;
    std::string              table_path;

private:
    std::unordered_map<std::string, unsigned int> commands;
    
public:
    CMD();
    CMD(std::vector<std::string>& arguments);
    ~CMD();

private:
    /* Local protected variables
       are initialized */
    void initVars(std::vector<std::string>& args, const unsigned int argn);

    /*  Existing commands are 
        mapped and initialized */
    void initializeCommands();

    /* Handle the command */
    void handle();
};

class Drop: public CMD 
{
public:
    Drop();
    ~Drop();
};

class Use: public CMD
{
public:
    Use();
    ~Use();
};

class Alter: public CMD
{
public:
    Alter();
    ~Alter();
};

#endif // CMD_H_