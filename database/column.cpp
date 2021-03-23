#include "column.h"

template<> Column<int>::Column(std::string column, std::vector<int> elements)
{
    this->column_name = column;
    this->elements = elements;
    this->data_type = 0;
}

template<> Column<float>::Column(std::string column, std::vector<float> elements)
{
    this->column_name = column;
    this->elements = elements;
    this->data_type = 1;
}

template<> Column<char>::Column(std::string column, std::vector<char> elements)
{
    this->column_name = column;
    this->elements = elements;
    this->data_type = 2;
}

template<> Column<std::string>::Column(std::string column, std::vector<std::string> elements, size_t max)
{
    this->column_name = column;
    this->elements = elements;
    this->data_type = 3;
    this->CHAR_MAX = max;
}

template<> bool Column<int>::insertElement(int el)
{
    try 
    {
        this->elements.emplace_back(el);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return false;
    }
    return true;
}

template<> bool Column<float>::insertElement(float el)
{
    try 
    {
        this->elements.emplace_back(el);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return false;
    }
    return true;
}

template<> bool Column<char>::insertElement(char el)
{
    try 
    {
        this->elements.emplace_back(el);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return false;
    }
    return true;
}

template<> bool Column<std::string>::insertElement(std::string el)
{
    try 
    {
        this->elements.emplace_back(el);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return false;
    }
    return true;
}

template<> std::unordered_set<size_t> Column<int>::filterElements(const std::string& op, int val)
{
    std::unordered_set<size_t> res;
    size_t index = 0;

    if (op == "=") {
        for (auto& e : this->elements) {
            if (e == val) {
                res.insert(index);
            }
            index++;
        }
    }
    else if (op == "!=") 
    {
        for (auto& e : this->elements) {
            if (e != val) {
                res.insert(index);
            }
            index++;
        }
    }
    else if (op == ">") 
    {
        for (auto& e : this->elements) {
            if (e > val) {
                res.insert(index);
            }
            index++;
        }
    }
    else if (op == ">=") 
    {
        for (auto& e : this->elements) {
            if (e >= val) {
                res.insert(index);
            }
            index++;
        }
    }
    else if (op == "<") 
    {
        for (auto& e : this->elements) {
            if (e < val) {
                res.insert(index);
            }
            index++;
        }
    }
    else if (op == "<=") 
    {
        for (auto& e : this->elements) {
            if (e <= val) {
                res.insert(index);
            }
            index++;
        }
    }

    return res;
}

template<> std::unordered_set<size_t> Column<float>::filterElements(const std::string& op, float val)
{
    std::unordered_set<size_t> res;
    size_t index = 0;

    if (op == "=") {
        for (auto& e : this->elements) {
            if (e == val) {
                res.insert(index);
            }
            index++;
        }
    }
    else if (op == "!=") 
    {
        for (auto& e : this->elements) {
            if (e != val) {
                res.insert(index);
            }
            index++;
        }
    }
    else if (op == ">") 
    {
        for (auto& e : this->elements) {
            if (e > val) {
                res.insert(index);
            }
            index++;
        }
    }
    else if (op == ">=") 
    {
        for (auto& e : this->elements) {
            if (e >= val) {
                res.insert(index);
            }
            index++;
        }
    }
    else if (op == "<") 
    {
        for (auto& e : this->elements) {
            if (e < val) {
                res.insert(index);
            }
            index++;
        }
    }
    else if (op == "<=") 
    {
        for (auto& e : this->elements) {
            if (e <= val) {
                res.insert(index);
            }
            index++;
        }
    }

    return res;
}

template<> std::unordered_set<size_t> Column<char>::filterElements(const std::string& op, char val)
{
    std::unordered_set<size_t> res;
    size_t index = 0;

    if (op == "=") {
        for (auto& e : this->elements) {
            if (e == val) {
                res.insert(index);
            }
            index++;
        }
    }
    else if (op == "!=") 
    {
        for (auto& e : this->elements) {
            if (e != val) {
                res.insert(index);
            }
            index++;
        }
    }
    else if (op == ">") 
    {
        for (auto& e : this->elements) {
            if (_toUpper(e) > _toUpper(val)) {
                res.insert(index);
            }
            index++;
        }
    }
    else if (op == ">=") 
    {
        for (auto& e : this->elements) {
            if (_toUpper(e) >= _toUpper(val)) {
                res.insert(index);
            }
            index++;
        }
    }
    else if (op == "<") 
    {
        for (auto& e : this->elements) {
            if (_toUpper(e) < _toUpper(val)) {
                res.insert(index);
            }
            index++;
        }
    }
    else if (op == "<=") 
    {
        for (auto& e : this->elements) {
            if (_toUpper(e) <= _toUpper(val)) {
                res.insert(index);
            }
            index++;
        }
    }

    return res;
}

template<> std::unordered_set<size_t> Column<std::string>::filterElements(const std::string& op, std::string val)
{
    std::unordered_set<size_t> res;
    size_t index = 0;

    if (op == "=") {
        for (auto& e : this->elements) {
            if (e.compare(val) == 0) 
            {
                res.insert(index);
            }
            index++;
        }
    }
    else if (op == "!=") 
    {
        for (auto& e : this->elements) {
            if (e.compare(val) != 0) {
                res.insert(index);
            }
            index++;
        }
    }
    else if (op == ">") 
    {
        for (auto& e : this->elements) 
        {
            if (_toUpper(e).compare(_toUpper(val)) > 0 ) 
            {
                res.insert(index);
            }
            index++;
        }
    }
    else if (op == ">=") 
    {
        for (auto& e : this->elements) {
            if ((e.compare(val) == 0) || _toUpper(e).compare(_toUpper(val)) > 0) 
            {
                res.insert(index);
            }
            index++;
        }
    }
    else if (op == "<") 
    {
        for (auto& e : this->elements) {
            if (_toUpper(e).compare(_toUpper(val)) < 0) 
            {
                res.insert(index);
            }
            index++;
        }
    }
    else if (op == "<=") 
    {
        for (auto& e : this->elements) {
            if ((e.compare(val) == 0) || _toUpper(e).compare(_toUpper(val)) < 0) 
            {
                res.insert(index);
            }
            index++;
        }
    }

    return res;
}

template <> size_t Column<int>::updateElementsOnIndex(const std::unordered_set<size_t>& indices, const int& val)
{
    // Set a maximum range for updating elements
    size_t max_size = this->elements.size();

    // Initialize count: the number of elements we update
    size_t count = 0;

    // Iterate over all indecies we want to update
    for (auto index : indices)
    {
        // If the index is in range, update to given value and increment count
        if (index < max_size) {
            this->elements[index] = val;
            ++count;
        }
    }

    // Return the result
    return count;
}

template <> size_t Column<float>::updateElementsOnIndex(const std::unordered_set<size_t>& indices, const float& val)
{
    // Set a maximum range for updating elements
    size_t max_size = this->elements.size();

    // Initialize count: the number of elements we update
    size_t count = 0;

    // Iterate over all indecies we want to update
    for (auto index : indices)
    {
        // If the index is in range, update to given value and increment count
        if (index < max_size) {
            this->elements[index] = val;
            ++count;
        }
    }

    // Return the result
    return count;
}

template <> size_t Column<char>::updateElementsOnIndex(const std::unordered_set<size_t>& indices, const char& val)
{
    // Set a maximum range for updating elements
    size_t max_size = this->elements.size();

    // Initialize count: the number of elements we update
    size_t count = 0;

    // Iterate over all indecies we want to update
    for (auto index : indices)
    {
        // If the index is in range, update to given value and increment count
        if (index < max_size) {
            this->elements[index] = val;
            ++count;
        }
    }

    // Return the result
    return count;
}

template <> size_t Column<std::string>::updateElementsOnIndex(const std::unordered_set<size_t>& indices, const std::string& val)
{
    // Set a maximum range for updating elements
    size_t max_size = this->elements.size();

    // Initialize count: the number of elements we update
    size_t count = 0;

    // Iterate over all indecies we want to update
    for (auto index : indices)
    {
        // If the index is in range, update to given value and increment count
        if (index < max_size) {
            this->elements[index] = val;
            ++count;
        }
    }

    // Return the result
    return count;
}

template <> bool Column<int>::deleteElement(const size_t index)
{
    try 
    {
        // Get an iterator to the position we want to delete
        std::vector<int>::const_iterator e = this->elements.begin() + index;

        // Erase that element
        this->elements.erase(e);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return false;
    }
    
    return true;
}

template <> bool Column<float>::deleteElement(const size_t index)
{
    try 
    {
        // Get an iterator to the position we want to delete
        std::vector<float>::const_iterator e = this->elements.begin() + index;

        // Erase that element
        this->elements.erase(e);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return false;
    }
    
    return true;   
}

template <> bool Column<char>::deleteElement(const size_t index)
{
    try 
    {
        // Get an iterator to the position we want to delete
        std::vector<char>::const_iterator e = this->elements.begin() + index;

        // Erase that element
        this->elements.erase(e);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return false;
    }
    
    return true;      
}

template <> bool Column<std::string>::deleteElement(const size_t index)
{
    try 
    {
        // Get an iterator to the position we want to delete
        std::vector<std::string>::const_iterator e = this->elements.begin() + index;

        // Erase that element
        this->elements.erase(e);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return false;
    }

    return true;
}
