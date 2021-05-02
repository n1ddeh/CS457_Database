/**
 * File: database.cpp
 * Author: Mark Minkoff
 * Functionality: Function definitions for file database.h
 * 
 * */

#include "database.h"

Database::Database(const std::string& database, const fs::path& path, const fs::path& path_metadata) : database_name(database), path(path), path_metadata(path_metadata), transaction_mode(false) {
    this->writeMetadata();
}

Database::Database() : database_name("undefined") {}

Database::~Database() {}

bool Database::tableExists(const std::string& table_name)
{
    bool count = this->tables.count(table_name);

    if (count) return count; 

    // Iterate over all tables if there any match
    std::string capital_table_name = _toUpper(table_name);
    for (std::pair<std::string, std::shared_ptr<Table>> table : this->tables ) 
    {
        if (_toUpper(std::get<0>(table)) == capital_table_name) {
            return true;
        }
    }
    return false;
}

bool Database::createTable(std::string table_name, std::vector<std::pair<std::string, std::string>> columns)
{
    fs::path table_path = this->path, table_metadata_path = this->path;

    table_path += "/"; table_path += table_name; table_path += "/"; 
    table_metadata_path = table_path;

    fs::create_directories(table_path);
    
    table_path += table_name; table_path += ".csv";
    table_metadata_path += table_name; table_metadata_path += ".txt";

    // Create and open the table file
    std::ofstream table_file(table_path, std::ofstream::out);

    for (auto& c : columns) {
        table_file << c.first << ' ' << c.second << ',';
    }

    // Insert new line
    table_file << '\n';

    // Close the file stream
    table_file.close();

    // Create a new table
    std::shared_ptr<Table> new_table = std::make_shared<Table>(table_name, columns, table_path, table_metadata_path);

    // Insert the table into memory
    this->tables.insert(std::make_pair(table_name, new_table));

    this->writeMetadata();
    
    return true;
}

bool Database::dropTable(const std::string& table_name)
{
    if (tableExists(table_name))
    {
        this->tables.erase(table_name);
        fs::remove( this->tables.at(table_name)->getPath() );
        return true;
    }
    return false;
}

bool Database::printTableColumnInfo(const std::string& table_name)
{
    if (tableExists(table_name)) {
        std::shared_ptr<Table> table = getTable(table_name);
        
        return true;
    }

    // Iterate over all tables if there any match
    std::string capital_table_name = _toUpper(table_name);
    for (std::pair<std::string, std::shared_ptr<Table>> table : this->tables ) 
    {
        if (_toUpper(std::get<0>(table)) == capital_table_name) {
            
            return true;
        }
    }

    return false;
}

std::shared_ptr<Table> Database::getTable(const std::string& table_name)
{
    if (this->tables.count(table_name)) {
        return this->tables.at(table_name);
    }

    // Iterate over all tables if there any match
    std::string capital_table_name = _toUpper(table_name);
    for (std::pair<std::string, std::shared_ptr<Table>> table : this->tables ) 
    {
        if (_toUpper(std::get<0>(table)) == capital_table_name) {
            return std::get<1>(table);
        }
    }

    return nullptr;
}

bool Database::addColumnsToTable(const std::string& table_name, std::vector<std::pair<std::string, std::string>> columns)
{
    std::shared_ptr<Table> table = getTable(table_name);
    for (int i = 0; i < columns.size(); i++)
    {

    }
    return true;
}

bool Database::queryTables (
    const std::pair<std::string, std::string>& table1,
    const std::pair<std::string, std::string>& table2,
    const std::pair<bool, bool>& lr_val,
    const bool inner,
    const std::vector<std::string>& statement )
{
    // If the tables do NOT exist, do nothing and return false.
    if (!this->tableExists(table1.first)) { std::cout << "-- !Failed to query " << table1.first << " because it does not exist\n"; return false; }
    if (!this->tableExists(table2.first)) { std::cout << "-- !Failed to query " << table2.first << " because it does not exist\n"; return false; }

    // If the query statement is not correct, do nothing and return false.
    if (statement.size() != 4) { std::cout << "-- !Failed query. Invalid query statement\n"; return false; }
    if (_toUpper(statement[0]) != "WHERE" && _toUpper(statement[0]) != "ON") { std::cout << "-- !Failed query. Invalid query statement. The query statement starts with 'WHERE' or 'ON'\n"; return false; }

    // If the operator is not valid, do nothing and return false.
    if (!_isValidOperator(statement[2])) { std::cout << "-- !Failed to query tables. Invalid operator " << statement[2] << "Did you mean '='?\n"; return false; }
    const std::string opr = statement[2];

    std::vector<std::string> table_select1 = _split(statement[1], '.');
    std::vector<std::string> table_select2 = _split(statement[3], '.');

    if (table_select1.size() != 2 && table_select1[0] != table1.second) { std::cout << "-- !Failed to query tables. Invalid syntax in query statement.\n"; return false; }
    if (table_select2.size() != 2 && table_select2[0] != table2.second) { std::cout << "-- !Failed to query tables. Invalid syntax in query statement.\n"; return false; }

    // Initialize pointers to each table
    std::shared_ptr<Table> table1_ptr = this->getTable(table1.first);
    std::shared_ptr<Table> table2_ptr = this->getTable(table2.first);

    // Get the index of each column to query
    size_t col1_index = table1_ptr->columnIndexFromName(table_select1[1]);
    size_t col2_index = table2_ptr->columnIndexFromName(table_select2[1]);

    // If the column index was not found for either table, do nothing and return false.
    if (col1_index == -1) { std::cout << "-- !Failed to query tables. Column " << table1.second << " does not exist in table " << table1.first << "\n"; return false; } 
    if (col2_index == -1) { std::cout << "-- !Failed to query tables. Column " << table2.second << " does not exist in table " << table2.first << "\n"; return false; }

    // Get the data type of each column.
    size_t column1_data_type = table1_ptr->getColumnType(table_select1[1]);
    size_t column2_data_type = table2_ptr->getColumnType(table_select2[1]);

    // If the columns are NOT the same data type, do nothing and return false.
    if (column1_data_type != column2_data_type) { std::cout << "-- !Failed to query tables. Columns are not the same data type.\n"; return false; }

    if (column1_data_type == 0) {
        auto column1 = table1_ptr->selectColumnInt(table_select1[1]);
        auto column2 = table2_ptr->selectColumnInt(table_select2[1]);

        std::unordered_map<size_t, std::vector<size_t>> mapping1;
        std::unordered_map<size_t, std::vector<size_t>> mapping2;

        // Right Join
        if (!lr_val.first && lr_val.second) 
        {
            mapping1 = queryColumnsInt(column2, column1, opr);
            this->printQuery(table2_ptr, table1_ptr, mapping1, mapping2, inner);
        }
        // Left Join
        else if (lr_val.first && !lr_val.second)
        {
            mapping1 = queryColumnsInt(column1, column2, opr);
            this->printQuery(table1_ptr, table2_ptr, mapping1, mapping2, inner);
        }
        // Full Join
        else if (lr_val.first && lr_val.second)
        {
            mapping1 = queryColumnsInt(column1, column2, opr);
            mapping2 = queryColumnsInt(column2, column1, opr);
            
            this->printQuery(table1_ptr, table2_ptr, mapping1, mapping2, inner);
        }        
    }
    else if (column1_data_type == 1) { // Float Columns
        auto column1 = table1_ptr->selectColumnFloat(table_select1[1]);
        auto column2 = table2_ptr->selectColumnFloat(table_select2[1]);

        std::unordered_map<size_t, std::vector<size_t>> mapping1;
        std::unordered_map<size_t, std::vector<size_t>> mapping2;

        // Right Join
        if (!lr_val.first && lr_val.second) 
        {
            mapping1 = queryColumnsFloat(column2, column1, opr);
            this->printQuery(table2_ptr, table1_ptr, mapping1, mapping2, inner);
        }
        // Left Join
        else if (lr_val.first && !lr_val.second)
        {
            mapping1 = queryColumnsFloat(column1, column2, opr);
            this->printQuery(table1_ptr, table2_ptr, mapping1, mapping2, inner);
        }
        // Full Join
        else if (lr_val.first && lr_val.second)
        {
            mapping1 = queryColumnsFloat(column1, column2, opr);
            mapping2 = queryColumnsFloat(column2, column1, opr);
            this->printQuery(table1_ptr, table2_ptr, mapping1, mapping2, inner);
        }
    }
    else if (column1_data_type == 2) { // Char Columns
        auto column1 = table1_ptr->selectColumnChar(table_select1[1]);
        auto column2 = table2_ptr->selectColumnChar(table_select2[1]);

        std::unordered_map<size_t, std::vector<size_t>> mapping1;
        std::unordered_map<size_t, std::vector<size_t>> mapping2;

        // Right Join
        if (!lr_val.first && lr_val.second) 
        {
            mapping1 = queryColumnsChar(column2, column1, opr);
            this->printQuery(table2_ptr, table1_ptr, mapping1, mapping2, inner);
        }
        // Left Join
        else if (lr_val.first && !lr_val.second)
        {
            mapping1 = queryColumnsChar(column1, column2, opr);
            this->printQuery(table1_ptr, table2_ptr, mapping1, mapping2, inner);
        }
        // Full Join
        else if (lr_val.first && lr_val.second)
        {
            mapping1 = queryColumnsChar(column1, column2, opr);
            mapping2 = queryColumnsChar(column2, column1, opr);
            this->printQuery(table1_ptr, table2_ptr, mapping1, mapping2, inner);
        }
    }
    else if (column1_data_type == 3) { // String Columns
        auto column1 = table1_ptr->selectColumnString(table_select1[1]);
        auto column2 = table2_ptr->selectColumnString(table_select2[1]);

        std::unordered_map<size_t, std::vector<size_t>> mapping1;
        std::unordered_map<size_t, std::vector<size_t>> mapping2;

        // Right Join
        if (!lr_val.first && lr_val.second) 
        {
            mapping1 = queryColumnsString(column2, column1, opr);
            this->printQuery(table2_ptr, table1_ptr, mapping1, mapping2, inner);
        }
        // Left Join
        else if (lr_val.first && !lr_val.second)
        {
            mapping1 = queryColumnsString(column1, column2, opr);
            this->printQuery(table1_ptr, table2_ptr, mapping1, mapping2, inner);
        }
        // Full Join
        else if (lr_val.first && lr_val.second)
        {
            mapping1 = queryColumnsString(column1, column2, opr);
            mapping2 = queryColumnsString(column2, column1, opr);
            this->printQuery(table1_ptr, table2_ptr, mapping1, mapping2, inner);
        }
    }
    else {
        std::cout << "-- !Failed to query tables. Unsupported data type.\n";
        return false;
    }

    return true;
}

std::unordered_map<size_t, std::vector<size_t>> Database::queryColumnsInt(
    std::shared_ptr<Column<int>> col1, 
    std::shared_ptr<Column<int>> col2, 
    const std::string& op
)
{   
    // Initialize resulting container
    std::unordered_map<size_t, std::vector<size_t>> res;

    // If operator is not valid or columns are null or left and right flags are both false, return an empty result.
    if(!_isValidOperator(op) || !col1 || !col2) return res;

    std::vector<int> elements1 = col1->getElements();
    std::vector<int> elements2 = col2->getElements();

    if (op == "=") { // Equality operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (elements1[i] == elements2[j]) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }
    }
    else if (op == "!=") { // Inequality operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (elements1[i] != elements2[j]) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }
    }
    else if (op == ">") { // Greater operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (elements1[i] > elements2[j]) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }
    }
    else if (op == ">=") { // Greater or equal operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (elements1[i] >= elements2[j]) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }
    }
    else if (op == "<") { // Less operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (elements1[i] < elements2[j]) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }
    }
    else if (op == "<=") { // Less or equal operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (elements1[i] <= elements2[j]) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }
    }

    return res;
}

std::unordered_map<size_t, std::vector<size_t>> Database::queryColumnsFloat(
    std::shared_ptr<Column<float>> col1, 
    std::shared_ptr<Column<float>> col2, 
    const std::string& op
)
{
    // Initialize resulting container
    std::unordered_map<size_t, std::vector<size_t>> res;

    std::vector<float> elements1 = col1->getElements();
    std::vector<float> elements2 = col2->getElements();

    // If operator is not valid or columns are null or left and right flags are both false, return an empty result.
    if(!_isValidOperator(op) || !col1 || !col2) return res;

    if (op == "=") { // Equality operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (elements1[i] == elements2[j]) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }
    }
    else if (op == "!=") { // Inequality operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (elements1[i] != elements2[j]) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }
    }
    else if (op == ">") { // Greater operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (elements1[i] > elements2[j]) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }
    }
    else if (op == ">=") { // Greater or equal operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (elements1[i] >= elements2[j]) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }
    }
    else if (op == "<") { // Less operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (elements1[i] < elements2[j]) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }
    }
    else if (op == "<=") { // Less or equal operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (elements1[i] <= elements2[j]) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }
    }

    return res;
}

std::unordered_map<size_t, std::vector<size_t>> Database::queryColumnsChar(
    std::shared_ptr<Column<char>> col1, 
    std::shared_ptr<Column<char>> col2, 
    const std::string& op
)
{
    // Initialize resulting container
    std::unordered_map<size_t, std::vector<size_t>> res;

    std::vector<char> elements1 = col1->getElements();
    std::vector<char> elements2 = col2->getElements();

    // If operator is not valid or columns are null or left and right flags are both false, return an empty result.
    if(!_isValidOperator(op) || !col1 || !col2) return res;

    if (op == "=") { // Equality operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (elements1[i] == elements2[j]) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }
    }
    else if (op == "!=") { // Inequality operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (elements1[i] != elements2[j]) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }
    }
    else if (op == ">") { // Greater operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (_toUpper(elements1[i]) > _toUpper(elements2[j])) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }
    }
    else if (op == ">=") { // Greater or equal operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (_toUpper(elements1[i]) > _toUpper(elements2[j]) || elements1[i] == elements2[j]) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }     
    }
    else if (op == "<") { // Less operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (_toUpper(elements1[i]) < _toUpper(elements2[j])) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }       
    }
    else if (op == "<=") { // Less or equal operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (_toUpper(elements1[i]) < _toUpper(elements2[j]) || elements1[i] == elements2[j]) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }            
    }

    return res;
}

std::unordered_map<size_t, std::vector<size_t>> Database::queryColumnsString(
    std::shared_ptr<Column<std::string>> col1, 
    std::shared_ptr<Column<std::string>> col2, 
    const std::string& op
)
{
    std::vector<std::string> elements1 = col1->getElements();
    std::vector<std::string> elements2 = col2->getElements();

    // Initialize resulting container
    std::unordered_map<size_t, std::vector<size_t>> res;

    // If operator is not valid or columns are null or left and right flags are both false, return an empty result.
    if(!_isValidOperator(op) || !col1 || !col2) return res;

    if (op == "=") { // Equality operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check 
                if (elements1[i].compare(elements2[j]) == 0)
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }
    }
    else if (op == "!=") { // Inequality operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (elements1[i].compare(elements2[j]) != 0)
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }
    }
    else if (op == ">") { // Greater operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (_toUpper(elements1[i]).compare(_toUpper(elements2[j])) > 0 ) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }
    }
    else if (op == ">=") { // Greater or equal operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (_toUpper(elements1[i]).compare(_toUpper(elements2[j])) > 0 || elements1[i].compare(elements2[j]) == 0) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }
    }
    else if (op == "<") { // Less operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (_toUpper(elements1[i]).compare(_toUpper(elements2[j])) < 0 ) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }
    }
    else if (op == "<=") { // Less or equal operator
        // Iterate over every element in the first column.
        for (size_t i = 0; i < elements1.size(); ++i) 
        {
            // If an element from the first column has NOT been mapped, map it.
            if (!res.count(i)) res.insert( std::make_pair(i, std::vector<size_t>()) );

            // Iterate over every element in the second column.
            for (size_t j = 0; j < elements2.size(); j++) 
            {
                // Perform check
                if (_toUpper(elements1[i]).compare(_toUpper(elements2[j])) < 0 || elements1[i].compare(elements2[j]) == 0) 
                {
                    // Store matching row
                    res.at(i).emplace_back(j);
                }
            }
        }
    }

    return res;
}

bool Database::printQuery(
        std::shared_ptr<Table> table1, 
        std::shared_ptr<Table> table2,
        std::unordered_map<size_t, std::vector<size_t>> map1,
        std::unordered_map<size_t, std::vector<size_t>> map2,
        bool inner
    )
{
    auto meta1 = table1->getMetaData();
    auto meta2 = table2->getMetaData();
    size_t counter = 0;

    // Print column meta data of col1 and col2.
    std::cout << "-- ";
    for (auto& c : meta1) {
        std::cout << c.first << " " << c.second;
        if (counter != (meta1.size() - 1)) std::cout << " | ";
        counter++;
    }
    if (meta2.size()) std::cout << " | ";
    for (auto& c : meta2) {
        std::cout << c.first << " " << c.second;
        if (counter != (meta1.size() - 1)) std::cout << " | ";
        counter++;
    }

    // Initialize a container for rows with no matches (used for outer joins)
    std::vector<size_t> no_match;

    std::cout << "\n";
    for (auto& m : map1) {
        for (auto& r: m.second) {
            std::cout << "-- ";
            table1->printRow(m.first);
            table2->printRow(r);
            std::cout << "\n";
        }
        if (m.second.empty()) no_match.emplace_back(m.first);
    }
    if (!inner) {
        for (auto& r : no_match) {
            std::cout << "-- ";
            table1->printRow(r);
            std::cout << "\n";
        }
        no_match.clear();
    }
    for (auto& m : map2) {
        for (auto& r: m.second) {
            std::cout << "-- ";
            table1->printRow(m.first);
            table2->printRow(r);
            std::cout << "\n";
        }
        if (m.second.empty()) no_match.emplace_back(m.first);
    }
    if (!inner) {
        for (auto& r : no_match) {
            std::cout << "-- ";
            table1->printRow(r);
            std::cout << "\n";
        }
    }

    return true;
}

bool Database::writeMetadata()
{
    const fs::path path = this->getPathMetadata();

    std::ofstream metadata_file(path, std::ofstream::out | std::ofstream::trunc);

    try {
        metadata_file << "table_name: " << this->getDatabaseName() << "\n";

        metadata_file << "tables: ";
        for (auto & t : this->getTables()) {
            metadata_file << t.first << ",";
        }
        metadata_file << "\n";

        metadata_file << "database_path: " << std::string(this->path.u8string()) << "\n";
        metadata_file << "metadata_path: " << std::string(this->path_metadata.u8string()) << "\n";

        metadata_file << "transaction_mode: " << this->getTransaction() ? "true" : "false";
        metadata_file << "\n";

    } catch(const std::exception& e) {
        std::cerr << e.what() << "\n";
        if (metadata_file.is_open()) {
            metadata_file.close();
        }
        return false;
    }

    if (metadata_file.is_open()) {
        metadata_file.close();
    }

    return true;
}