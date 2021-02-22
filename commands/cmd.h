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

    /*  Check if a specified 
        cmd exists  */
    bool checkCMD(const std::string& command);

    unsigned int get_CMD_ID(const std::string& command);

private:
    /* Local protected variables
       are initialized */
    void initVars(std::vector<std::string>& args, const unsigned int argn);

    /*  Existing commands are 
        mapped and initialized */
    void initializeCommands();

};

#endif // CMD_H_