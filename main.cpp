#include "database/SQL.h"


int main(int argc, char* argv[])
{
    std::cout << "CS457 PA3\n\n";

    if (argc > 1) {
        std::string filePath = argv[1];
        std::unique_ptr<SQL> client = std::make_unique<SQL>(filePath);
    }
    else {
        std::unique_ptr<SQL> client = std::make_unique<SQL>();
    }
    
    return 0;
}