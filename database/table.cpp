#include "table.h"

// Constructor
Table::Table(std::string table, std::vector<std::pair<std::string, std::string>> column_meta_data) : 
    table_name(table), column_count(0), column_meta_data(column_meta_data) 
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

bool Table::insertRow(const std::vector<std::string>& row)
{
    size_t col_index = 0;

    const unsigned int argn = row.size();
    
    if (argn != column_count) 
    {
        std::cout << "-- INSERT INTO parameter count (" << argn << ") does not equal the number of columns (" << column_count << ") in table " << table_name << "\n";
        return false;
    }

    /*  For every variable in the row, check if
        the variable can be converted to the type
        required by the column. **/
    for (auto& var : row)
    {
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
        else 
        {
            std::cout << "-- Programmer error in Table::insertRow\n";
        }
        ++col_index;
    }
    
    this->row_count++;

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
