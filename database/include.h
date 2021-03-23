#ifndef INCLUDE_H_
#define INCLUDE_H_

// STL FTW
#include <algorithm>
#include <cmath>
#include <ctype.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <numeric>
#include <map>
#include <queue>
#include <regex>
#include <sstream>
#include <string>
#include <stack>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

// Converts a string to upper case
static std::string _toUpper(std::string str)
{
    // Create a temp string of size 'str'
    std::string up;
    up.reserve(str.size());

    // Convert each letter of 'str' to uppercase and append to 'up'
    for (auto& c : str) up += ::toupper(c);
    
    // Return the uppercase string
    return up;
}

// Joins a vector of strings by a delimeter
static std::string _join(std::vector<std::string> stringVector, std::string delim = "")
{
    return std::accumulate(stringVector.begin(), stringVector.end(), delim);
}
// Returns a subvector
template<typename T>
static auto _subVector(const std::vector<T>& vec, typename std::vector<T>::const_iterator left, typename std::vector<T>::const_iterator right) -> std::vector<T>
{
    // Return a vector in range of a left iterator to a right iterator
    return std::vector<T>(left, right);
}

// Rounds a floating point number by a set number of significant digits
static float _roundFloat(float num, unsigned int sig_figs) 
{
    float power_of_10 = std::pow(10, sig_figs);

    float res = std::round(num * power_of_10)  / power_of_10;

    return res;
}

static std::string _trimEnd(std::string source) 
{
    source.erase(std::find_if(source.rbegin(), source.rend(), [](char c) 
    {
        return !std::isspace(static_cast<unsigned char>(c));
    }).base(), source.end());
    return source;
}

static std::string _trimStart(std::string source) 
{
    source.erase(source.begin(), std::find_if(source.begin(), source.end(), [](char c) 
    {
        return !std::isspace(static_cast<unsigned char>(c));
    }));
    return source;
}

static std::string _trim(std::string source) 
{
    return _trimEnd(_trimStart(source));
}

static bool _isValidOperator(const std::string& op)
{
    if (
        op == "="  || 
        op == "!=" || 
        op == "<"  || 
        op == "<=" || 
        op == ">"  || 
        op == ">="
    ) return true;
    return false;
}

static char _toUpper(const char& c)
{
    char res = c;
    if (res >= 'a' && res <= 'z') {
        res = res - 32;
    }
    return res;
}

#endif // INCLUDE_H_
