#include "database/SQL.h"


int main(void)
{
    std::cout << "CS457 PA1\n\n";

    // Launch instance of SQL (Mark Minkoff's SQL for CS 482)
    std::unique_ptr<SQL> client = std::make_unique<SQL>();

    return 0;
}