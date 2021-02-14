#ifndef CMD_H_
#define CMD_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

class CMD
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


class Create: public CMD
{
public:
    Create();
    ~Create();
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