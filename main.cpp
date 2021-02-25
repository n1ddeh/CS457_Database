#include "database/SQL.h"


int main(void)
{
    std::cout << "SQL CLI" << std::endl;

    // Launch instance of SQL (Mark Minkoff's SQL for CS 482)
    std::unique_ptr<SQL> client = std::make_unique<SQL>();

    return 0;
}