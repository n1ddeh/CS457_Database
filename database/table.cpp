/**
 * File: table.cpp
 * Author: Mark Minkoff
 * Functionality: Function definitions for file table.cpp
 * 
 * */

#include "table.h"

// Constructor
Table::Table(std::string table, std::vector<std::pair<std::string, std::string>> column_meta_data, fs::path path, fs::path path_metadata) : 
    table_name(table), column_count(0), row_count(0), column_meta_data(column_meta_data), path(path), locked("false"), path_metadata(path_metadata)
    {
        for (auto& col: column_meta_data)
        {
            std::string col_name = std::get<0>(col);
            std::string col_type = _toUpper(std::get<1>(col));

            if (col_type == "INT")
            {
                createIntColumn(col_name);
            }
            else if (col_type == "FLOAT")
            {
                createFloatColumn(col_name);
            }
            else if (col_type == "CHAR")
            {
                createCharColumn(col_name);
            }
            else if (col_type.size() > 9 && col_type.substr(0, 8) == "VARCHAR(" && col_type.back() == ')')
            {
                std::string size;
                for (int i = 8; i < col_type.size() - 1; i++) 
                {
                    size += col_type[i];
                }
                size_t max = std::stoul(size);
                createVarCharColumn(col_name, max);
            }
            else {
                std::cout << "-- Type Error: Unknown type " << col_type << "\n";
            }
        }

        this->writeMetadata();
    }

Table::~Table() {}

bool Table::createIntColumn(std::string column) 
{
    std::variant<std::shared_ptr<Column<int>>, std::shared_ptr<Column<float>>, std::shared_ptr<Column<char>>, std::shared_ptr<Column<std::string>>> data;

    try
    {
        data = std::make_shared<Column<int>>(column, std::vector<int>());

        this->columns.emplace_back(data);

        this->incrementColumnCount();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return false;
    }
    return true;
}

bool Table::createFloatColumn(std::string column) 
{
    std::variant<std::shared_ptr<Column<int>>, std::shared_ptr<Column<float>>, std::shared_ptr<Column<char>>, std::shared_ptr<Column<std::string>>> data;

    try
    {
        data = std::make_shared<Column<float>>(column, std::vector<float>());

        this->columns.emplace_back(data);

        this->incrementColumnCount();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return false;
    }
    return true;
}

bool Table::createCharColumn(std::string column) 
{
    std::variant<std::shared_ptr<Column<int>>, std::shared_ptr<Column<float>>, std::shared_ptr<Column<char>>, std::shared_ptr<Column<std::string>>> data;

    try
    {
        data = std::make_shared<Column<char>>(column, std::vector<char>());

        this->columns.emplace_back(data);

        this->incrementColumnCount();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return false;
    }
    return true;
}

bool Table::createVarCharColumn(std::string column, size_t max) 
{

    std::variant<std::shared_ptr<Column<int>>, std::shared_ptr<Column<float>>, std::shared_ptr<Column<char>>, std::shared_ptr<Column<std::string>>> data;

    try
    {
        data = std::make_shared<Column<std::string>>(column, std::vector<std::string>(), max);

        this->columns.emplace_back(data);

        this->incrementColumnCount();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return false;
    }
    return true;
}

bool Table::insertRow(const std::vector<std::string>& row, bool write)
{
    size_t col_index = 0;

    const unsigned int argn = row.size();
    
    if (argn != column_count) 
    {
        std::cout << "-- INSERT INTO parameter count (" << argn << ") does not equal the number of columns (" << column_count << ") in table " << table_name << "\n";
        return false;
    }

    // Open the table file and set pointer to end of the file
    std::ofstream table_file(this->path, std::ofstream::out | std::ofstream::app);

    /*  For every variable in the row, check if
        the variable can be converted to the type
        required by the column. **/
    for (auto& var : row)
    {
        if (write) table_file << var << ',';
        if (auto col = std::get_if<std::shared_ptr<Column<int>>>(&(this->columns[col_index])))              // INT Type   
        {
            // Get a pointer to the column
            std::shared_ptr<Column<int>> column = *col;
            
            // Convert value to integer
            int val = std::stoi(var);

            // Insert value into column
            column->insertElement(val);
        }
        else if (auto col = std::get_if<std::shared_ptr<Column<float>>>(&(this->columns[col_index])))       // FLOAT Type
        {
            // Get a pointer to the column
            std::shared_ptr<Column<float>> column = *col;

            // Initialize the amount of significant figures to round to
            unsigned int sig_figs = 0;

            size_t dec = var.find_first_of('.');

            // Check if decimal found in string
            if (dec != std::string::npos) 
            {
                // Decimal found, so count sig figs
                for (dec++; dec < var.size(); dec++) sig_figs++;
            }
            
            // Convert value to float
            float val = std::stof(var);

            if (sig_figs) {
                val = _roundFloat(val, sig_figs);
            }

            // Insert value into column
            column->insertElement(val);
        }
        else if (auto col = std::get_if<std::shared_ptr<Column<char>>>(&(this->columns[col_index])))        // CHAR Type
        {
            // Get a pointer to the column
            std::shared_ptr<Column<char>> column = *col;
            
            // Convert value to char
            char val = var[0];

            // Insert value into column
            column->insertElement(val);
        }
        else if (auto col = std::get_if<std::shared_ptr<Column<std::string>>>(&(this->columns[col_index]))) // VARCHAR Type
        {
            // Get a pointer to the column
            std::shared_ptr<Column<std::string>> column = *col;

            size_t max = column->getCharMax();

            // If the size of the string is greater than the max char count, then only insert a range of var
            if (var.size() > max) {
                std::string val = std::string(var.begin(), var.begin() + column->getCharMax());
                column->insertElement(val);
            }

            // Insert value into column
            column->insertElement(var);
        }
        else {
            std::cout << "-- Programmer error in Table::insertRow\n";
        }
        ++col_index;
    }
    // Insert new line
    if (write) table_file << '\n';

    // Close the file
    if (table_file.is_open()) table_file.close();

    // Increment row count
    this->row_count = ((std::get<0>(this->columns[0]))->getElements()).size();

    // Update metadata
    this->writeMetadata();

    return true;
}

bool Table::printAll()
{
    // Print column meta data
    std::cout << "-- ";
    for (size_t i = 0; i < this->column_meta_data.size(); i++)
    {
        std::cout << std::get<0>(this->column_meta_data[i]) << " " << std::get<1>(this->column_meta_data[i]);
        if (i != (this->column_meta_data.size() - 1)) std::cout << " | ";
    }
    std::cout << "\n";

    if (!this->row_count) return true;

    size_t col_index = 0;
    size_t row_index = 0;

    for (; row_index < this->row_count; ++row_index, col_index=0)
    {
        for (; col_index < this->column_count; ++col_index)
        {
            std::string to_print;
            if (auto col = std::get_if<std::shared_ptr<Column<int>>>(&(this->columns[col_index])))
            {
                // Get a pointer to the column
                std::shared_ptr<Column<int>> column = *col;

                to_print = std::to_string(column->getElements()[row_index]);
            }
            else if (auto col = std::get_if<std::shared_ptr<Column<float>>>(&(this->columns[col_index])))
            {
                // Get a pointer to the column
                std::shared_ptr<Column<float>> column = *col;

                to_print = std::to_string(column->getElements()[row_index]);
            }
            else if (auto col = std::get_if<std::shared_ptr<Column<char>>>(&(this->columns[col_index])))
            {
                // Get a pointer to the column
                std::shared_ptr<Column<char>> column = *col;

                to_print = column->getElements()[row_index];
            }
            else if (auto col = std::get_if<std::shared_ptr<Column<std::string>>>(&(this->columns[col_index])))
            {
                // Get a pointer to the column
                std::shared_ptr<Column<std::string>> column = *col;

                to_print = column->getElements()[row_index];
            }
            
            if (col_index == 0) std::cout << "-- ";
            std::cout << to_print;
            if (col_index != (column_count - 1)) std::cout << " | ";
        }
        std::cout << "\n";
    }
    return true;
}

long int Table::columnIndexFromName(const std::string& column_name)
{
    long int index = 0;
    for (auto& col_data : this->column_meta_data) {
        std::string name = std::get<0>(this->column_meta_data[index]);
        if (_toUpper(name) == _toUpper(column_name)) return index;
        index++;
    }
    return (long int) -1;
}

bool Table::updateColumnSet(
    const std::string& column_to_update,
    const std::string& column_to_search,
    const std::string& value_to_update,
    const std::string& value_to_search,
    const std::string& op,
    const bool mode
)
{
    long int update_colum_index = columnIndexFromName(column_to_update);
    if (update_colum_index == (long int)-1) { std::cout << "-- !Failed to update table " << table_name << " because column " << column_to_update << " does not exist.\n"; return false; }

    long int search_colum_index = (column_to_update == column_to_search) ? update_colum_index : columnIndexFromName(column_to_search);
    if (search_colum_index == (long int)-1) { std::cout << "-- !Failed to update table " << table_name << " because column " << search_colum_index << " does not exist.\n"; return false; }

    size_t rows_affected = 0;

    // initialize container for the indicies we want to update in 'column_to_update'
    std::unordered_set<size_t> elements_to_update;

    try
    {
        if (auto col = std::get_if<std::shared_ptr<Column<int>>>(&(this->columns[search_colum_index])))
        {
            // Get a pointer to the column
            std::shared_ptr<Column<int>> column = *col;

            // Search column 
            elements_to_update = column->filterElements(op, std::stoi(value_to_search));
        }
        else if (auto col = std::get_if<std::shared_ptr<Column<float>>>(&(this->columns[search_colum_index])))
        {
            // Get a pointer to the column
            std::shared_ptr<Column<float>> column = *col;

            elements_to_update = column->filterElements(op, std::stof(value_to_search));
        }
        else if (auto col = std::get_if<std::shared_ptr<Column<char>>>(&(this->columns[search_colum_index])))
        {
            // Get a pointer to the column
            std::shared_ptr<Column<char>> column = *col;

            elements_to_update = column->filterElements(op, value_to_search[0]);
        }
        else if (auto col = std::get_if<std::shared_ptr<Column<std::string>>>(&(this->columns[search_colum_index])))
        {
            // Get a pointer to the column
            std::shared_ptr<Column<std::string>> column = *col;

            elements_to_update = column->filterElements(op, value_to_search);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return false;
    }

    if (mode == false) {
        std::cout << "-- " << elements_to_update.size() << " records modified.\n";
        return true;
    }

    try 
    {
        if (!elements_to_update.empty()) 
        {
            if (auto col = std::get_if<std::shared_ptr<Column<int>>>(&(this->columns[update_colum_index]))) {
                // Get a pointer to the column
                std::shared_ptr<Column<int>> column = *col;

                // Update the column based on provided indicies
                rows_affected = column->updateElementsOnIndex(elements_to_update, std::stoi(value_to_update));
            }
            else if (auto col = std::get_if<std::shared_ptr<Column<float>>>(&(this->columns[update_colum_index]))) {
                // Get a pointer to the column
                std::shared_ptr<Column<float>> column = *col;

                // Update the column based on provided indicies
                rows_affected = column->updateElementsOnIndex(elements_to_update, std::stof(value_to_update));            
            }
            else if (auto col = std::get_if<std::shared_ptr<Column<char>>>(&(this->columns[update_colum_index])))
            {
                // Get a pointer to the column
                std::shared_ptr<Column<char>> column = *col;

                // Update the column based on provided indicies
                rows_affected = column->updateElementsOnIndex(elements_to_update, value_to_update[0]);
            }
            else if (auto col = std::get_if<std::shared_ptr<Column<std::string>>>(&(this->columns[update_colum_index])))
            {
                // Get a pointer to the column
                std::shared_ptr<Column<std::string>> column = *col;

                // Update the column based on provided indicies
                rows_affected = column->updateElementsOnIndex(elements_to_update, value_to_update);   
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return false;
    }

    std::cout << "-- " << rows_affected << " records modified.\n";

    return true;
}

bool Table::deleteRow(const size_t row)
{
    if (row >= this->row_count) {
        std::cout << "-- !Cannot delete row " << row << " from table " << this->table_name << ".\n";
        return false;
    }

    try 
    {
        for (size_t index = 0; index < this->column_count; index++)
        {
            if (auto col = std::get_if<std::shared_ptr<Column<int>>>(&(this->columns[index]))) 
            {
                // Get a pointer to the column
                std::shared_ptr<Column<int>> column = *col;

                column->deleteElement(row);
            }
            else if (auto col = std::get_if<std::shared_ptr<Column<float>>>(&(this->columns[index]))) 
            {
                // Get a pointer to the column
                std::shared_ptr<Column<float>> column = *col;

                column->deleteElement(row);
            }
            else if (auto col = std::get_if<std::shared_ptr<Column<char>>>(&(this->columns[index]))) 
            {
                // Get a pointer to the column
                std::shared_ptr<Column<char>> column = *col;

                column->deleteElement(row);
            }
            else if (auto col = std::get_if<std::shared_ptr<Column<std::string>>>(&(this->columns[index]))) 
            {
                // Get a pointer to the column
                std::shared_ptr<Column<std::string>> column = *col;

                column->deleteElement(row);
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return false;
    }

    this->decrementRowCount();

    return true;
}

bool Table::deleteFromTable(const std::string& column_to_search, const std::string& value_to_search, const std::string& opr)
{
    long int search_column_index = columnIndexFromName(column_to_search);
    if (search_column_index == (long int)-1) { std::cout << "-- !Failed to delete from table " << this->table_name << " because column " << column_to_search << " does not exist.\n"; return false; }

    std::unordered_set<size_t> indicies_to_delete;

    if (auto col = std::get_if<std::shared_ptr<Column<int>>>(&(this->columns[search_column_index])))
    {
        // Get a pointer to the column
        std::shared_ptr<Column<int>> column = *col;

        // Get the indicies of the rows we want to delete  
        indicies_to_delete = column->filterElements(opr, std::stoi(value_to_search));
    }
    else if (auto col = std::get_if<std::shared_ptr<Column<float>>>(&(this->columns[search_column_index])))
    {
        // Get a pointer to the column
        std::shared_ptr<Column<float>> column = *col;

        // Get the indicies of the rows we want to delete  
        indicies_to_delete = column->filterElements(opr, std::stof(value_to_search));
    }
    else if (auto col = std::get_if<std::shared_ptr<Column<char>>>(&(this->columns[search_column_index])))
    {
        // Get a pointer to the column
        std::shared_ptr<Column<char>> column = *col;

        // Get the indicies of the rows we want to delete  
        indicies_to_delete = column->filterElements(opr, value_to_search[0]);
    }
    else if (auto col = std::get_if<std::shared_ptr<Column<std::string>>>(&(this->columns[search_column_index])))
    {
        // Get a pointer to the column
        std::shared_ptr<Column<std::string>> column = *col;

        // Get the indicies of the rows we want to delete  
        indicies_to_delete = column->filterElements(opr, value_to_search);
    }

    // Initialize the number of rows deleted
    size_t count = 0;

    // Check if there are rows to delete
    if (!indicies_to_delete.empty())
    {
        // For every row index, delete the row and increment the count
        for (auto& index : indicies_to_delete)
        {
            if (this->deleteRow(index)) ++count;
        }
    }

    std::cout << "-- " << count << " records deleted.\n";

    return true; 
}


bool Table::selectColumns(
        const std::vector<std::string>& columns,
        const std::string& column_to_query,
        const std::string& value_to_query, 
        const std::string& opr
    ) 
{
    std::vector<size_t> column_indicies;

    // Ensure that each column exists in this table
    for (auto col : columns)
    {
        column_indicies.emplace_back(columnIndexFromName(col));
        if (!this->columnExists(col)) {
            std::cout << "-- !Failed to query table " << this->table_name << " because column " << col << "does not exist.\n";
            return false;
        }
    }

    std::unordered_set<size_t> indicies_to_select;

    long int query_column_index = columnIndexFromName(column_to_query);
    if (query_column_index == (long int)-1) { std::cout << "-- !Failed to query from table " << this->table_name << " because column " << query_column_index << " does not exist.\n"; return false; }

    auto variant_col = &(this->columns[query_column_index]);
    if (auto col = std::get_if<std::shared_ptr<Column<int>>>(variant_col))
    {
        // Get a pointer to the column
        std::shared_ptr<Column<int>> column = *col;

        indicies_to_select = column->filterElements(opr, std::stoi(value_to_query));
    }
    else if (auto col = std::get_if<std::shared_ptr<Column<float>>>(variant_col))
    {
        // Get a pointer to the column
        std::shared_ptr<Column<float>> column = *col;

        indicies_to_select = column->filterElements(opr, std::stof(value_to_query));
    }
    else if (auto col = std::get_if<std::shared_ptr<Column<char>>>(variant_col))
    {
        // Get a pointer to the column
        std::shared_ptr<Column<char>> column = *col;

        indicies_to_select = column->filterElements(opr, value_to_query[0]);
    }
    else if (auto col = std::get_if<std::shared_ptr<Column<std::string>>>(variant_col))
    {
        // Get a pointer to the column
        std::shared_ptr<Column<std::string>> column = *col;

        indicies_to_select = column->filterElements(opr, value_to_query);
    }

    if (!indicies_to_select.empty()) 
    {
        // Print column meta data
        std::cout << "-- ";
        for (size_t index : column_indicies)
        {
            if (index <= this->columnCount())
            {
                std::cout << std::get<0>(this->column_meta_data[index]) << " " << std::get<1>(this->column_meta_data[index]);
                std::cout << " | ";
            }
            
        }
        std::cout << "\n";

        for (size_t row_index : indicies_to_select)
        {
            std::cout << "-- ";
            for (size_t col_index : column_indicies)
            {
                if (auto col = std::get_if<std::shared_ptr<Column<int>>>(&(this->columns[col_index])))
                {
                    // Get a pointer to the column
                    std::shared_ptr<Column<int>> column = *col;

                    std::cout << column->getElements()[row_index];
                    std::cout << " | ";
                }
                else if (auto col = std::get_if<std::shared_ptr<Column<float>>>(&(this->columns[col_index])))
                {
                    // Get a pointer to the column
                    std::shared_ptr<Column<float>> column = *col;

                    std::cout << column->getElements()[row_index];
                    std::cout << " | ";
                }
                else if (auto col = std::get_if<std::shared_ptr<Column<char>>>(&(this->columns[col_index])))
                {
                    // Get a pointer to the column
                    std::shared_ptr<Column<char>> column = *col;

                    std::cout << column->getElements()[row_index];
                    std::cout << " | ";
                }
                else if (auto col = std::get_if<std::shared_ptr<Column<std::string>>>(&(this->columns[col_index])))
                {
                    // Get a pointer to the column
                    std::shared_ptr<Column<std::string>> column = *col;

                    std::cout << column->getElements()[row_index];
                    std::cout << " | ";
                } 
            }
            std::cout << "\n";
        }
    }

    return true;
}

bool Table::columnExists(const std::string& column_name)
{
    for (auto& col : this->column_meta_data) {
        if (_toUpper(column_name) == _toUpper(std::get<0>(col))) return true;
    }

    return false;
}

size_t Table::getColumnType(const std::string& column_name)
{
    size_t column_index = this->columnIndexFromName(column_name);
    if (column_index == -1 ) return -1;

    if (auto col = std::get_if<std::shared_ptr<Column<int>>>(&(this->columns[column_index])))
    {
        // Get a pointer to the column
        std::shared_ptr<Column<int>> column = *col;

        // Return the data type
        return column->getDataType();
    }
    else if (auto col = std::get_if<std::shared_ptr<Column<float>>>(&(this->columns[column_index])))
    {
        // Get a pointer to the column
        std::shared_ptr<Column<float>> column = *col;

        // Return the data type
        return column->getDataType();
    }
    else if (auto col = std::get_if<std::shared_ptr<Column<char>>>(&(this->columns[column_index])))
    {
        // Get a pointer to the column
        std::shared_ptr<Column<char>> column = *col;

        // Return the data type
        return column->getDataType();
    }
    else if (auto col = std::get_if<std::shared_ptr<Column<std::string>>>(&(this->columns[column_index])))
    {
        // Get a pointer to the column
        std::shared_ptr<Column<std::string>> column = *col;

        // Return the data type
        return column->getDataType();
    }
    return -1;
}

std::shared_ptr<Column<int>> Table::selectColumnInt(const std::string& column_name) 
{
    std::shared_ptr<Column<int>> column;

    size_t index = this->columnIndexFromName(column_name);

    if (index == -1 || this->getColumnType(column_name) != (size_t)0) {
        return column;
    }

    if (auto col = std::get_if<std::shared_ptr<Column<int>>>(&(this->columns[index])))
    {
        column = *col;
    }

    return column;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                             
std::shared_ptr<Column<float>> Table::selectColumnFloat(const std::string& column_name)
{
    std::shared_ptr<Column<float>> column;

    size_t index = this->columnIndexFromName(column_name);

    if (index == -1 || this->getColumnType(column_name) != (size_t)1) return column;

    if (auto col = std::get_if<std::shared_ptr<Column<float>>>(&(this->columns[index])))
    {
        column = *col;
    }

    return column;
}

std::shared_ptr<Column<char>> Table::selectColumnChar(const std::string& column_name)
{
    std::shared_ptr<Column<char>> column;

    size_t index = this->columnIndexFromName(column_name);

    if (index == -1 || this->getColumnType(column_name) != (size_t)2) return column;

    if (auto col = std::get_if<std::shared_ptr<Column<char>>>(&(this->columns[index])))
    {
        column = *col;
    }

    return column;
}

std::shared_ptr<Column<std::string>> Table::selectColumnString(const std::string& column_name)
{
    std::shared_ptr<Column<std::string>> column;

    size_t index = this->columnIndexFromName(column_name);

    if (index == -1 || this->getColumnType(column_name) != (size_t)3) return column;

    if (auto col = std::get_if<std::shared_ptr<Column<std::string>>>(&(this->columns[index])))
    {
        column = *col;
    }

    return column;
}

bool Table::printRow(const size_t row) {
    if (row >= this->row_count) return false;

    size_t counter = 0;
    
    try {
        // Iterate over every column
        for (size_t i = 0; i < this->column_count; ++i) {
            if (auto col = std::get_if<std::shared_ptr<Column<int>>>(&(this->columns[i]))) {
                // Get a pointer to the column
                std::shared_ptr<Column<int>> column = *col;
                
                // Print the value
                std::cout << column->getElements()[row];
            }
            else if (auto col = std::get_if<std::shared_ptr<Column<float>>>(&(this->columns[i]))) {
                // Get a pointer to the column
                std::shared_ptr<Column<float>> column = *col;
                
                // Print the value
                std::cout << column->getElements()[row];
            }
            else if (auto col = std::get_if<std::shared_ptr<Column<char>>>(&(this->columns[i]))) {
                // Get a pointer to the column
                std::shared_ptr<Column<char>> column = *col;
                
                // Print the value
                std::cout << column->getElements()[row];
            }
            else if (auto col = std::get_if<std::shared_ptr<Column<std::string>>>(&(this->columns[i]))) {
                // Get a pointer to the column
                std::shared_ptr<Column<std::string>> column = *col;
                
                // Print the value
                std::cout << column->getElements()[row];
            }

            if (counter != (this->columnCount() - 1)) std::cout << " | ";
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return false;
    }
    return true;
}

bool Table::writeMetadata() 
{
    const fs::path path = this->getPathMetadata();

    std::ofstream metadata_file(path, std::ofstream::out | std::ofstream::trunc);
    try {
        metadata_file << "table_name: " << this->getTable() << "\n";

        metadata_file << "columns: ";
        for (auto& c : this->getMetaData()) {
            metadata_file << c.first << " " << c.second << ",";
        }
        metadata_file << "\n";

        metadata_file << "column_count: " << this->column_count << "\n";
        metadata_file << "row_count: " << this->row_count << "\n";

        metadata_file << "table_path: " << std::string(this->path.u8string()) << "\n";
        metadata_file << "metadata_path: " << std::string(this->path_metadata.u8string()) << "\n";

        metadata_file << "locked: " << this->getLocked();
        
        metadata_file << "\n";
    }
    catch(const std::exception& e) {
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

void Table::applyMetadata(const TableMetadata& md )
{
    this->setTableName(md.table_name);
    this->setLocked(md.locked);
}

bool Table::writeCSV()
{
    std::ofstream file(this->getPath(), std::ofstream::out | std::ofstream::trunc);

    auto headers = this->getMetaData();
    for (auto& col : headers) {
        file << col.first << " " << col.second << ",";
    }
    file << "\n";

    size_t rows = this->getRowCount();

    try {
        for (size_t row = 0; row < rows; row++)
        {
            // Iterate over every column
            for (size_t i = 0; i < this->column_count; ++i) {
                if (auto col = std::get_if<std::shared_ptr<Column<int>>>(&(this->columns[i]))) {
                    // Get a pointer to the column
                    std::shared_ptr<Column<int>> column = *col;
                    
                    // Print the value
                    file << column->getElements()[row] << ",";
                }
                else if (auto col = std::get_if<std::shared_ptr<Column<float>>>(&(this->columns[i]))) {
                    // Get a pointer to the column
                    std::shared_ptr<Column<float>> column = *col;
                    
                    // Print the value
                    file << column->getElements()[row] << ",";
                }
                else if (auto col = std::get_if<std::shared_ptr<Column<char>>>(&(this->columns[i]))) {
                    // Get a pointer to the column
                    std::shared_ptr<Column<char>> column = *col;
                    
                    // Print the value
                    file << column->getElements()[row] << ",";
                }
                else if (auto col = std::get_if<std::shared_ptr<Column<std::string>>>(&(this->columns[i]))) {
                    // Get a pointer to the column
                    std::shared_ptr<Column<std::string>> column = *col;
                    
                    // Print the value
                    file << column->getElements()[row] << ",";
                }
            }
            file << "\n";
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return false;
    }
    return true;
}